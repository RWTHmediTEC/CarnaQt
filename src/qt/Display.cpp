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
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/SpatialMovement.h>
#include <Carna/base/GLContext.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Log.h>
#include <Carna/base/ProjectionControl.h>
#include <Carna/base/CameraControl.h>
#include <Carna/presets/MeshColorCodingStage.h>
#include <QGLContext>
#include <QMouseEvent>
#include <QWheelEvent>
#include <set>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Display :: Details
// ----------------------------------------------------------------------------------

struct Display::Details
{
    Details( const std::function< void( base::FrameRenderer& ) >& );
    const std::function< void( base::FrameRenderer& ) > setupRenderer;

    static std::set< const Display* > sharingDisplays;
    static const Display* pickSharingDisplay();
    
    typedef base::QGLContextAdapter< QGLContext > GLContext;

    std::unique_ptr< GLContext > glc;
    std::unique_ptr< base::FrameRenderer > renderer;
    bool glInitializationFinished;
    
    ViewportMode vpMode;
    
    base::Camera* cam;
    base::Node* root;
    std::unique_ptr< base::Aggregation< base::CameraControl > > camControl;
    std::unique_ptr< base::Aggregation< base::ProjectionControl > > projControl;

    bool mouseInteraction;
    QPoint mousepos;
    float radiansPerPixel;
    float axialMovementSpeed;
    
    presets::MeshColorCodingStage* mccs;
    std::unique_ptr< base::SpatialMovement > spatialMovement;
    
    void updateProjection( Display& );
    bool fitSquare() const;
};


std::set< const Display* > Display::Details::sharingDisplays = std::set< const Display* >();


Display::Details::Details( const std::function< void( base::FrameRenderer& ) >& setupRenderer )
    : setupRenderer( setupRenderer )
    , glInitializationFinished( false )
    , vpMode( fitAuto )
    , cam( nullptr )
    , root( nullptr )
    , radiansPerPixel( DEFAULT_ROTATION_SPEED )
    , axialMovementSpeed( DEFAULT_AXIAL_MOVEMENT_SPEED )
    , mccs( nullptr )
{
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
        base::math::Matrix4f projection;
        display.projectionControl().updateProjection( projection );
        display.camera().setProjection( projection );
        
        /* Redraw if required.
         */
        if( glInitializationFinished )
        {
            display.updateGL();
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
        return projControl.get() != nullptr && projControl->get() != nullptr;
        
    default:
        CARNA_FAIL( "Unknown ViewportMode value." );
        
    }
}



// ----------------------------------------------------------------------------------
// Display
// ----------------------------------------------------------------------------------

const float Display::DEFAULT_ROTATION_SPEED       = 1e-2f;
const float Display::DEFAULT_AXIAL_MOVEMENT_SPEED = 1e+0f;


Display::Display( const std::function< void( base::FrameRenderer& ) >& setupRenderer, QWidget* parent )
    : QGLWidget( parent, Details::pickSharingDisplay() )
    , pimpl( new Details( setupRenderer ) )
{
    Details::sharingDisplays.insert( this );
}


Display::~Display()
{
    Details::sharingDisplays.erase( this );
    if( pimpl->glc.get() != nullptr )
    {
        pimpl->glc->makeActive();
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
    pimpl->root = nullptr;
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
    CARNA_ASSERT( pimpl->glc.get() == nullptr );
    pimpl->glc.reset( new Details::GLContext() );
}


void Display::resizeGL( int w, int h )
{
    CARNA_ASSERT( pimpl->glc.get() != nullptr );
    const unsigned int width  = static_cast< unsigned int >( w );
    const unsigned int height = static_cast< unsigned int >( h );
    if( pimpl->renderer == nullptr )
    {
        pimpl->renderer.reset( new base::FrameRenderer( *pimpl->glc, width, height, pimpl->fitSquare() ) );
        pimpl->setupRenderer( *pimpl->renderer );
        pimpl->mccs = pimpl->renderer->findStage< presets::MeshColorCodingStage >().get();
        pimpl->updateProjection( *this );
        pimpl->glInitializationFinished = true;
    }
    else
    {
        pimpl->renderer->reshape( width, height, pimpl->fitSquare() );
        pimpl->updateProjection( *this );
    }
}


void Display::paintGL()
{
    CARNA_ASSERT( pimpl->renderer.get() != nullptr );
    
    if( pimpl->cam == nullptr )
    {
        base::Log::instance().record( base::Log::debug, "Display has no camera but should render." );
    }
    else
    {
        if( pimpl->root == nullptr )
        {
            pimpl->root = &pimpl->cam->findRoot();
        }
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


void Display::mousePressEvent( QMouseEvent* ev )
{
    /* First, try to pick object at clicked location.
     */
    const base::Geometry* picked = nullptr;
    if( pimpl->mccs != nullptr )
    {
        picked = pimpl->mccs->pick( ev->x(), ev->y() ).get();
    }
    
    /* Initiate camera rotation if nothing was picked.
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


void Display::mouseMoveEvent( QMouseEvent* ev )
{
    if( pimpl->mouseInteraction )
    {
        if( pimpl->spatialMovement.get() == nullptr && hasCamera() && hasCameraControl() )
        {
            /* Camera rotation is going on.
             */
            const int dx = ( ev->x() - pimpl->mousepos.x() );
            const int dy = ( ev->y() - pimpl->mousepos.y() );
            pimpl->mousepos = ev->pos();

            if( dx != 0 || dy != 0 )
            {
                cameraControl().rotateHorizontally( dx * pimpl->radiansPerPixel );
                cameraControl().rotateVertically  ( dx * pimpl->radiansPerPixel );
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
                updateGL();
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
        ev->accept();
    }
}
    
    
void Display::setCameraControl( base::Aggregation< base::CameraControl >* camControl )
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
    
    
void Display::setProjectionControl( base::Aggregation< base::ProjectionControl >* projControl )
{
    pimpl->projControl.reset( projControl );
    pimpl->updateProjection( *this );
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



}  // namespace Carna :: qt

}  // namespace Carna

