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

#include <Carna/qt/MPRDisplay.h>
#include <Carna/qt/MPRStage.h>
#include <Carna/qt/MPRDataFeature.h>
#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/presets/OrthogonalControl.h>
#include <Carna/presets/CameraNavigationControl.h>
#include <Carna/helpers/FrameRendererHelper.h>
#include <Carna/base/Aggregation.h>
#include <Carna/base/Node.h>
#include <Carna/base/NodeListener.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Viewport.h>
#include <QVBoxLayout>
#include <QMouseEvent>

#include <Carna/base/text.h>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPRDisplay :: Details
// ----------------------------------------------------------------------------------

struct MPRDisplay::Details : public QObject
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    
    MPRDisplay& self;
    Details( MPRDisplay& self, const Parameters& params );
    MPR* mpr;
    MPRDataFeature planeData;
    MPRStage* mprRenderStage;

    Display* const display;
    static Display* createDisplay( const Parameters& params, MPRStage* mprRenderStage );
    
    base::math::Matrix4f pivotRotation;
    base::math::Matrix4f pivotBaseTransform;
    base::Node pivot;
    base::Geometry* const plane;
    base::Camera* const cam;
    const std::unique_ptr< presets::OrthogonalControl > projControl;
    void updatePivot();
    
    virtual bool eventFilter( QObject* obj, QEvent* ev ) override;
    void mouseMoveEvent( QMouseEvent* ev );
    void mousePressEvent( QMouseEvent* ev );
    void mouseReleaseEvent( QMouseEvent* ev );
    
    struct PlaneDragInfo;
    std::unique_ptr< PlaneDragInfo > currentPlane;
    
    struct PlaneMovement;
    std::unique_ptr< PlaneMovement > planeMovement;
};


struct MPRDisplay::Details::PlaneDragInfo
{
    PlaneDragInfo( const base::Geometry& plane, bool horizontal, int direction );
    base::Geometry& plane;
    const bool horizontal;
    const int direction;
};


MPRDisplay::Details::PlaneDragInfo::PlaneDragInfo( const base::Geometry& plane, bool horizontal, int direction )
    /* This 'const_cast' is okay because the plane belongs to another 'MPRDisplay'
     * that this one could just as well query from the common 'MPR', but this is
     * faster and easier.
     */
    : plane( const_cast< base::Geometry& >( plane ) )
    , horizontal( horizontal )
    , direction( direction )
{
}


struct MPRDisplay::Details::PlaneMovement
{
    int previousFrameCoordinate;
};


MPRDisplay::Details::Details( MPRDisplay& self, const Parameters& params )
    : self( self )
    , mpr( nullptr )
    , mprRenderStage( new MPRStage( params.geometryTypePlanes ) )
    , display( createDisplay( params, mprRenderStage ) )
    , plane( new base::Geometry( params.geometryTypePlanes ) )
    , cam( new base::Camera() )
    , projControl( new presets::OrthogonalControl( new presets::CameraNavigationControl() ) )
    , pivotRotation( base::math::identity4f() )
{
    /* Configure the 'pivot' node.
     */
    pivot.attachChild( plane );
    pivot.attachChild( cam );
    
    /* Configure the plane.
     */
    planeData.color = DEFAULT_PLANE_COLOR;
    plane->putFeature( MPRStage::ROLE_PLANE_DATA, planeData );
    
    /* Configure the camera.
     */
    projControl->setZoomStrength( 1e-2f );
    cam->localTransform = base::math::translation4f( 0, 0, params.visibleDistance / 2 );
    projControl->setMinimumVisibleDistance( 0 );
    projControl->setMaximumVisibleDistance( params.visibleDistance );
    projControl->setRotationEnabled( false );
    
    /* Configure the display.
     */
    display->setMouseTracking( true );
    display->setCamera( *cam );
    display->setCameraControl( new base::Aggregation< base::CameraControl >( *projControl ) );
    display->setProjectionControl( new base::Aggregation< base::ProjectionControl >( *projControl ) );
    display->installEventFilter( this );
    display->setLateralMovementSpeed( -1 );
}


bool MPRDisplay::Details::eventFilter( QObject* obj, QEvent* ev )
{
    switch( ev->type() )
    {
        case QEvent::Wheel:
        {
            display->updateProjection();
            break;
        }
        case QEvent::MouseMove:
        {
            mouseMoveEvent( static_cast< QMouseEvent* >( ev ) );
            break;
        }
        case QEvent::MouseButtonPress:
        {
            mousePressEvent( static_cast< QMouseEvent* >( ev ) );
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            mouseReleaseEvent( static_cast< QMouseEvent* >( ev ) );
            break;
        }
    }
    
    /* Process the event in default way.
     */
    return QObject::eventFilter( obj, ev );
}


void MPRDisplay::Details::mouseMoveEvent( QMouseEvent* ev )
{
    if( display->hasRenderer() )
    {
        /* Map the frame coordinates to clipping coordinates.
         */
        const base::Viewport& vp = display->renderer().viewport();
        const float clippingX =  ( ( static_cast< float >( ev->x() - vp.marginLeft() ) / vp.width () ) * 2 - 1 );
        const float clippingY = -( ( static_cast< float >( ev->y() - vp.marginTop () ) / vp.height() ) * 2 - 1 );
        
        if( planeMovement.get() == nullptr )
        {
            const unsigned int planeMargin = 10; // pixels
            
            /* Perform hit test with projected planes.
             */
            const MPRStage::ProjectedPlane& horizontal = mprRenderStage->horizontal();
            const MPRStage::ProjectedPlane& vertical   = mprRenderStage->  vertical();
            const float planeMarginH = static_cast< float >( planeMargin ) / ( vp. width() - 1 );
            const float planeMarginV = static_cast< float >( planeMargin ) / ( vp.height() - 1 );
            const float offsetV = horizontal.clippingCoordinate - clippingY;
            const float offsetH =   vertical.clippingCoordinate - clippingX;
            if( horizontal.plane != nullptr && std::abs( offsetV ) <= planeMarginV )
            {
                display->setCursor( Qt::SizeVerCursor );
                currentPlane.reset( new PlaneDragInfo( *horizontal.plane, true, horizontal.direction ) );
            }
            else
            if( vertical.plane != nullptr && std::abs( offsetH ) <= planeMarginH )
            {
                display->setCursor( Qt::SizeHorCursor );
                currentPlane.reset( new PlaneDragInfo( *vertical.plane, false, vertical.direction ) );
            }
            else
            {
                display->setCursor( Qt::ArrowCursor );
                currentPlane.reset();
            }
        }
        else
        if( currentPlane.get() != nullptr )
        {
            const int currentFrameCoordinate = currentPlane->horizontal ? ev->y() : ev->x();
            const int deltaFrameCoordinate = currentFrameCoordinate - planeMovement->previousFrameCoordinate;
            planeMovement->previousFrameCoordinate = currentFrameCoordinate;
            
            const int direction = currentPlane->direction * ( currentPlane->horizontal ? +1 : -1 );
            const float displacement = projControl->zoomFactor() * ( deltaFrameCoordinate * direction );
            currentPlane->plane.localTransform *= base::math::translation4f( 0, 0, displacement );
            currentPlane->plane.invalidate();
        }
    }
}


void MPRDisplay::Details::mousePressEvent( QMouseEvent* ev )
{
    if( ev->button() == Qt::LeftButton && currentPlane.get() != nullptr )
    {
        planeMovement.reset( new PlaneMovement() );
        planeMovement->previousFrameCoordinate = currentPlane->horizontal ? ev->y() : ev->x();
    }
}


void MPRDisplay::Details::mouseReleaseEvent( QMouseEvent* ev )
{
    planeMovement.reset();
}


Display* MPRDisplay::Details::createDisplay( const Parameters& params, MPRStage* mprRenderStage )
{
    FrameRendererFactory* const frFactory = new FrameRendererFactory();
    helpers::FrameRendererHelper< > frHelper( *frFactory );
    frHelper << new presets::CuttingPlanesStage( params.geometryTypeVolume, params.geometryTypePlanes );
    for( auto rsItr = params.extraRenderStages.begin(); rsItr != params.extraRenderStages.end(); ++rsItr )
    {
        frHelper << *rsItr;
    }
    frHelper.commit();
    frFactory->appendStage( mprRenderStage );
    Display* const display = new Display( frFactory );
    return display;
}


void MPRDisplay::Details::updatePivot()
{
    pivot.localTransform = pivotBaseTransform * pivotRotation;
}



// ----------------------------------------------------------------------------------
// MPRDisplay :: Parameters
// ----------------------------------------------------------------------------------

MPRDisplay::Parameters::Parameters( unsigned int geometryTypeVolume, unsigned int geometryTypePlanes )
    : geometryTypeVolume( geometryTypeVolume )
    , geometryTypePlanes( geometryTypePlanes )
    , unzoomedVisibleSideLength( DEFAULT_UNZOOMED_VISIBLE_SIDE_LENGTH )
    , visibleDistance( DEFAULT_VISIBLE_DISTANCE )
{
}



// ----------------------------------------------------------------------------------
// MPRDisplay
// ----------------------------------------------------------------------------------

const float MPRDisplay::DEFAULT_UNZOOMED_VISIBLE_SIDE_LENGTH = 500;
const float MPRDisplay::DEFAULT_VISIBLE_DISTANCE = 2000;
const base::Color MPRDisplay::DEFAULT_PLANE_COLOR( 255, 255, 255, 255 );


MPRDisplay::MPRDisplay( const Parameters& params, QWidget* parent )
    : QWidget( parent )
    , pimpl( new Details( *this, params ) )
    , parameters( params )
{
    setLayout( new QVBoxLayout() );
    this->layout()->addWidget( pimpl->display );
    this->layout()->setContentsMargins( 0, 0, 0, 0 );
    
    /* Resize this widget to its recommended minimum size if it is a distinct window.
     */
    if( parent == nullptr )
    {
        this->resize( this->minimumSizeHint() );
    }
}


MPRDisplay::~MPRDisplay()
{
    removeFromMPR();
    pimpl->display->removeEventFilter( pimpl.get() );
}


QSize MPRDisplay::minimumSizeHint() const
{
    return QSize( 400, 400 );
}


void MPRDisplay::setRotation( const base::math::Matrix3f& rotation )
{
    pimpl->pivotRotation.topLeftCorner( 3, 3 ) = rotation;
    pimpl->updatePivot();
    invalidate();
}


void MPRDisplay::invalidate()
{
    pimpl->display->invalidate();
}


void MPRDisplay::attachPivot( base::Node& to )
{
    to.attachChild( &pimpl->pivot );
    invalidate();
}


void MPRDisplay::detachPivot()
{
    pimpl->pivot.detachFromParent();
    invalidate();
}


void MPRDisplay::updatePivot( const base::math::Matrix4f& baseTransform )
{
    pimpl->pivotBaseTransform = baseTransform;
    pimpl->updatePivot();
    invalidate();
}


void MPRDisplay::setMPR( MPR& mpr )
{
    CARNA_ASSERT( parameters.geometryTypeVolume == mpr.geometryTypeVolume );
    if( pimpl->mpr != &mpr )
    {
        removeFromMPR();
        pimpl->mpr = &mpr;
        mpr.addDisplay( *this );
    }
}


void MPRDisplay::removeFromMPR()
{
    if( pimpl->mpr != nullptr )
    {
        pimpl->mpr->removeDisplay( *this );
        pimpl->mpr = nullptr;
    }
}


void MPRDisplay::setPlaneColor( const base::Color& color )
{
    pimpl->planeData.color = color;
}


const base::Color& MPRDisplay::planeColor() const
{
    return pimpl->planeData.color;
}



}  // namespace Carna :: qt

}  // namespace Carna
