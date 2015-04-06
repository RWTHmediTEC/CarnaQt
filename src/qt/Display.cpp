/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/base/NodeListener.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/SpatialMovement.h>
#include <Carna/base/GLContext.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Log.h>
#include <Carna/base/ProjectionControl.h>
#include <Carna/base/CameraControl.h>
#include <Carna/presets/MeshColorCodingStage.h>
#include <QGLContext>
#include <QGLFormat>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <set>
#include <typeinfo>

#ifndef _MSC_VER
#include <cxxabi.h> // for 'demangledName' function below
#endif

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// demangledName
// ----------------------------------------------------------------------------------

template< typename T >
std::string demangledName( const T& x )
{
    const char* name = typeid( x ).name();
#ifdef _MSC_VER
    return std::string( name );
#else
    int status;
    char* demangled = abi::__cxa_demangle( name, 0, 0, &status );
    const std::string str( demangled );
    free( demangled );
    return str;
#endif
}



// ----------------------------------------------------------------------------------
// Display :: Details
// ----------------------------------------------------------------------------------

struct Display::Details : public base::NodeListener
{
    Details( Display& self, FrameRendererFactory* rendererFactory );
    Display& self;
    bool invalidated;
    
    std::unique_ptr< FrameRendererFactory > rendererFactory;
    static std::map< const base::FrameRenderer*, Display* > displaysByRenderer;
    
    std::string logTag;

    static std::set< const Display* > sharingDisplays;
    static const Display* pickSharingDisplay();
    
    typedef base::QGLContextAdapter< QGLContext, QGLFormat > GLContext;

    std::unique_ptr< GLContext > glc;
    std::unique_ptr< base::FrameRenderer > renderer;
    
    ViewportMode vpMode;
    
    base::Camera* cam;
    base::Node* root;
    std::unique_ptr< base::Association< base::CameraControl > > camControl;
    std::unique_ptr< base::Association< base::ProjectionControl > > projControl;
    bool isProjectionUpdateRequested;
    void validateRoot();
    void invalidateRoot();

    bool mouseInteraction;
    QPoint mousepos;
    float radiansPerPixel;
    float axialMovementSpeed;
    float lateralMovementSpeed;
    
    presets::MeshColorCodingStage* mccs;
    std::unique_ptr< base::SpatialMovement > spatialMovement;
    
    void updateProjection( Display& );
    bool fitSquare() const;
    
    virtual void onNodeDelete( const base::Node& node ) override;
    virtual void onTreeChange( base::Node& node, bool inThisSubtree ) override;
    virtual void onTreeInvalidated( base::Node& subtree ) override;
};


std::map< const base::FrameRenderer*, Display* > Display::Details::displaysByRenderer = std::map< const base::FrameRenderer*, Display* >();
std::set< const Display* > Display::Details::sharingDisplays = std::set< const Display* >();


Display::Details::Details( Display& self, FrameRendererFactory* rendererFactory )
    : self( self )
    , invalidated( false )
    , rendererFactory( rendererFactory )
    , vpMode( fitAuto )
    , cam( nullptr )
    , root( nullptr )
    , camControl( nullptr )
    , projControl( nullptr )
    , isProjectionUpdateRequested( false )
    , mouseInteraction( false )
    , radiansPerPixel( DEFAULT_ROTATION_SPEED )
    , axialMovementSpeed( DEFAULT_AXIAL_MOVEMENT_SPEED )
    , lateralMovementSpeed( DEFAULT_LATERAL_MOVEMENT_SPEED )
    , mccs( nullptr )
{
    CARNA_ASSERT( rendererFactory != nullptr );
}


const Display* Display::Details::pickSharingDisplay()
{
    if( sharingDisplays.empty() )
    {
        return nullptr;
    }
    else
    {
        return *sharingDisplays.begin();
    }
}


void Display::Details::updateProjection( Display& display )
{
    if( display.hasCamera() && display.hasProjectionControl() && display.width() > 0 && display.height() > 0 )
    {
        const unsigned int width  = static_cast< unsigned int >( display. width() );
        const unsigned int height = static_cast< unsigned int >( display.height() );
        
        /* Update projection parameters.
         */
        if( fitSquare() )
        {
            const unsigned int vpSize = std::min( width, height );
            display.projectionControl().setViewportWidth ( vpSize );
            display.projectionControl().setViewportHeight( vpSize );
        }
        else
        {
            display.projectionControl().setViewportWidth (  width );
            display.projectionControl().setViewportHeight( height );
        }
        
        /* Fetch new projection matrix.
         */
        if( display.projectionControl().isUpdateAvailable() )
        {
            base::math::Matrix4f projection;
            display.projectionControl().updateProjection( projection );
            display.camera().setProjection( projection );
        }
    }
}


bool Display::Details::fitSquare() const
{
    switch( vpMode )
    {
    
    case Display::fitSquare:
        return true;
        
    case Display::fitFrame:
        return false;
        
    case Display::fitAuto:
        return projControl.get() == nullptr || projControl->get() == nullptr;
        
    default:
        CARNA_FAIL( "Unknown ViewportMode value." );
        
    }
}


void Display::Details::invalidateRoot()
{
    if( root != nullptr )
    {
        root->removeNodeListener( *this );
        root = nullptr;
    }
}


void Display::Details::validateRoot()
{
    if( root == nullptr )
    {
        root = &cam->findRoot();
        root->addNodeListener( *this );
    }
    else
    if( root->hasParent() )
    {
        invalidateRoot();
        validateRoot();
    }
}


void Display::Details::onNodeDelete( const base::Node& node )
{
    CARNA_ASSERT( &node == root );
    
    /* We do not use 'invalidateRoot' here because we are not allowed to remove the
     * listener from that dying node.
     */
    root = nullptr;
    self.invalidate();
}


void Display::Details::onTreeChange( base::Node& node, bool inThisSubtree )
{
    invalidateRoot();
    self.invalidate();
}


void Display::Details::onTreeInvalidated( base::Node& subtree )
{
    self.invalidate();
}



// ----------------------------------------------------------------------------------
// Display
// ----------------------------------------------------------------------------------

const float Display::DEFAULT_ROTATION_SPEED         = -3e-3f;
const float Display::DEFAULT_AXIAL_MOVEMENT_SPEED   = -1e-1f;
const float Display::DEFAULT_LATERAL_MOVEMENT_SPEED = -5e-1f;


Display::Display( FrameRendererFactory* rendererFactory, QWidget* parent )
    : QGLWidget( Details::GLContext::desiredFormat(), parent, Details::pickSharingDisplay() )
    , pimpl( new Details( *this, rendererFactory ) )
{
    Details::sharingDisplays.insert( this );
}


Display::~Display()
{
    pimpl->invalidateRoot();
    Details::sharingDisplays.erase( this );
    if( pimpl->renderer.get() != nullptr )
    {
        Details::displaysByRenderer.erase( pimpl->renderer.get() );
    }
}


void Display::setViewportMode( ViewportMode vpMode )
{
    if( vpMode != pimpl->vpMode )
    {
        pimpl->vpMode = vpMode;
        if( pimpl->renderer.get() != nullptr )
        {
            resizeGL( width(), height() );
            
            /* Repaint.
             */
            invalidate();
        }
    }
}


void Display::setCamera( base::Camera& cam )
{
    pimpl->cam = &cam;
    pimpl->updateProjection( *this );
    if( hasCameraControl() )
    {
        cameraControl().setCamera( cam );
    }
    
    /* We will search the root node the first time we render the frame.
     */
    pimpl->invalidateRoot();

    /* Repaint.
     */
    invalidate();
}


bool Display::hasCamera() const
{
    return pimpl->cam != nullptr;
}


base::Camera& Display::camera()
{
    return *pimpl->cam;
}


const base::Camera& Display::camera() const
{
    return *pimpl->cam;
}


void Display::initializeGL()
{
    CARNA_LOG_TAG_SCOPE( logTag() );
    CARNA_ASSERT( pimpl->glc.get() == nullptr );
    pimpl->glc.reset( new Details::GLContext() );
}


void Display::resizeGL( int w, int h )
{
    CARNA_LOG_TAG_SCOPE( logTag() );
    CARNA_ASSERT( pimpl->glc.get() != nullptr );
    const unsigned int width  = static_cast< unsigned int >( w );
    const unsigned int height = static_cast< unsigned int >( h );
    if( pimpl->renderer == nullptr )
    {
        pimpl->renderer.reset( pimpl->rendererFactory->createRenderer( *pimpl->glc, width, height, pimpl->fitSquare() ) );
        pimpl->rendererFactory.reset();
        pimpl->mccs = pimpl->renderer->findStage< presets::MeshColorCodingStage >().get();
        pimpl->updateProjection( *this );
        Details::displaysByRenderer[ pimpl->renderer.get() ] = this;
        
        /* Log debug message which stages are used.
         */
        std::stringstream msg;
        msg << "Initialized Display with following rendering stages:";
        for( std::size_t rsIdx = 0; rsIdx < pimpl->renderer->stages(); ++rsIdx )
        {
            msg << std::endl << "  "
                << ( rsIdx + 1 ) << ". "
                << demangledName( pimpl->renderer->stageAt( rsIdx ) );
        }
        base::Log::instance().record( base::Log::debug, msg.str() );
        
        /* Log debug message whether drag-&-drop is enabled.
         */
        if( pimpl->mccs != nullptr )
        {
            base::Log::instance().record
                ( base::Log::debug
                , "Drag-&-Drop behaviour for mesh-typed geometry ENABLED." );
        }
        else
        {
            base::Log::instance().record
                ( base::Log::debug
                , "Drag-&-Drop behaviour for mesh-typed geometry DISABLED." );
        }
    }
    else
    {
        pimpl->renderer->reshape( width, height, pimpl->fitSquare() );
        pimpl->updateProjection( *this );
    }
}


void Display::paintGL()
{
    CARNA_LOG_TAG_SCOPE( logTag() );
    CARNA_ASSERT( pimpl->renderer.get() != nullptr );
    
    if( pimpl->cam == nullptr )
    {
        base::Log::instance().record( base::Log::debug, "Display has no camera but should render." );
    }
    else
    {
        pimpl->glc->makeCurrent();
        if( pimpl->isProjectionUpdateRequested )
        {
            pimpl->updateProjection( *this );
            pimpl->isProjectionUpdateRequested = false;
        }
        pimpl->invalidated = false;
        pimpl->validateRoot();
        pimpl->renderer->render( *pimpl->cam, *pimpl->root );
    }
}


void Display::setRotationSpeed( float radiansPerPixel )
{
    pimpl->radiansPerPixel = radiansPerPixel;
}


void Display::setAxialMovementSpeed( float axialMovementSpeed )
{
    pimpl->axialMovementSpeed = axialMovementSpeed;
}


void Display::setLateralMovementSpeed( float lateralMovementSpeed )
{
    pimpl->lateralMovementSpeed = lateralMovementSpeed;
}


void Display::mousePressEvent( QMouseEvent* ev )
{
    if( ev->buttons() & Qt::LeftButton )
    {
        /* First, try to pick object at clicked location.
         */
        const base::Geometry* picked = nullptr;
        if( pimpl->mccs != nullptr )
        {
            picked = pimpl->mccs->pick( ev->x(), ev->y() ).get();
        }
        
        /* Initiate camera interaction if nothing was picked.
         */
        if( picked == nullptr )
        {
            pimpl->mousepos = ev->pos();
            pimpl->mouseInteraction = true;
            ev->accept();
        }
        else
        if( pimpl->renderer != nullptr && hasCamera() && hasCameraControl() )
        {
            /* Picking was successful! Initiate spatial movement.
             */
            base::Geometry& pickedGeometry = const_cast< base::Geometry& >( *picked );
            pimpl->spatialMovement.reset
                ( new base::SpatialMovement( pickedGeometry, ev->x(), ev->y(), pimpl->renderer->viewport(), *pimpl->cam ) );
            pimpl->mouseInteraction = true;
            ev->accept();
        }
    }
}


void Display::mouseMoveEvent( QMouseEvent* ev )
{
    if( pimpl->mouseInteraction )
    {
        if( pimpl->spatialMovement.get() == nullptr && hasCamera() && hasCameraControl() )
        {
            /* Camera interaction is going on.
             */
            const int dx = ( ev->x() - pimpl->mousepos.x() );
            const int dy = ( ev->y() - pimpl->mousepos.y() );
            pimpl->mousepos = ev->pos();

            if( dx != 0 || dy != 0 )
            {
                if( ev->modifiers() & Qt::ShiftModifier )
                {
                    cameraControl().moveLaterally( dx * pimpl->lateralMovementSpeed, -dy * pimpl->lateralMovementSpeed );
                }
                else
                {
                    cameraControl().rotateHorizontally( dx * pimpl->radiansPerPixel );
                    cameraControl().rotateVertically  ( dy * pimpl->radiansPerPixel );
                }
                updateGL();
                ev->accept();
            }
        }
        else
        if( pimpl->spatialMovement.get() != nullptr )
        {
            /* Spatial movement is going on. Update it.
             */
            if( pimpl->spatialMovement->update( ev->x(), ev->y() ) )
            {
                ev->accept();
            }
        }
    }
}


void Display::mouseReleaseEvent( QMouseEvent* ev )
{
    pimpl->mouseInteraction = false;
    pimpl->spatialMovement.reset();
    ev->accept();
}


void Display::wheelEvent( QWheelEvent* ev )
{
    if( hasCamera() && hasCameraControl() )
    {
        cameraControl().moveAxially( ev->delta() * pimpl->axialMovementSpeed );
        updateGL();
        ev->accept();
    }
}
    
    
void Display::setCameraControl( base::Association< base::CameraControl >* camControl )
{
    pimpl->camControl.reset( camControl );
    if( hasCamera() )
    {
        cameraControl().setCamera( *pimpl->cam );
    }
}


bool Display::hasCameraControl() const
{
    return pimpl->camControl.get() != nullptr && pimpl->camControl->get() != nullptr;
}


base::CameraControl& Display::cameraControl()
{
    CARNA_ASSERT( hasCameraControl() );
    return **pimpl->camControl;
}


const base::CameraControl& Display::cameraControl() const
{
    CARNA_ASSERT( hasCameraControl() );
    return **pimpl->camControl;
}
    
    
void Display::setProjectionControl( base::Association< base::ProjectionControl >* projControl )
{
    pimpl->projControl.reset( projControl );
    pimpl->updateProjection( *this );
    invalidate();
}


bool Display::hasProjectionControl() const
{
    return pimpl->projControl.get() != nullptr && pimpl->projControl->get() != nullptr;
}


base::ProjectionControl& Display::projectionControl()
{
    CARNA_ASSERT( hasProjectionControl() );
    return **pimpl->projControl;
}


const base::ProjectionControl& Display::projectionControl() const
{
    CARNA_ASSERT( hasProjectionControl() );
    return **pimpl->projControl;
}


bool Display::hasRenderer() const
{
    return pimpl->renderer.get() != nullptr;
}


base::FrameRenderer& Display::renderer()
{
    CARNA_ASSERT( hasRenderer() );
    return *pimpl->renderer;
}


const base::FrameRenderer& Display::renderer() const
{
    CARNA_ASSERT( hasRenderer() );
    return *pimpl->renderer;
}


void Display::setLogTag( const std::string& tag )
{
    pimpl->logTag = tag;
}


const std::string& Display::logTag() const
{
    return pimpl->logTag;
}


base::Aggregation< Display > Display::byRenderer( const base::FrameRenderer& renderer )
{
    const auto displayItr = Details::displaysByRenderer.find( &renderer );
    if( displayItr == Details::displaysByRenderer.end() )
    {
        return base::Aggregation< Display >::NULL_PTR;
    }
    else
    {
        return base::Aggregation< Display >( *displayItr->second );
    }
}


void Display::invalidate()
{
    if( !pimpl->invalidated && isVisible() )
    {
        QTimer::singleShot( 0, this, SLOT( updateGL() ) );
        pimpl->invalidated = true;
    }
}


void Display::updateProjection()
{
    pimpl->isProjectionUpdateRequested = true;
    invalidate();
}



}  // namespace Carna :: qt

}  // namespace Carna

