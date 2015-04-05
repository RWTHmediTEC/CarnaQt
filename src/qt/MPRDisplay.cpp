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
#include <QVBoxLayout>

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

    Display* const display;
    static Display* createDisplay( const Parameters& params );
    
    base::math::Matrix4f pivotRotation;
    base::math::Matrix4f pivotBaseTransform;
    base::Node pivot;
    base::Geometry* const plane;
    base::Camera* const cam;
    const std::unique_ptr< presets::OrthogonalControl > projControl;
    void updatePivot();
    
    virtual bool eventFilter( QObject* obj, QEvent* ev ) override;
};


MPRDisplay::Details::Details( MPRDisplay& self, const Parameters& params )
    : self( self )
    , mpr( nullptr )
    , display( createDisplay( params ) )
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
    }
    
    /* Process the event in default way.
     */
    return QObject::eventFilter( obj, ev );
}


Display* MPRDisplay::Details::createDisplay( const Parameters& params )
{
    FrameRendererFactory* const frFactory = new FrameRendererFactory();
    helpers::FrameRendererHelper< > frHelper( *frFactory );
    frHelper << new presets::CuttingPlanesStage( params.geometryTypeVolume, params.geometryTypePlanes );
    for( auto rsItr = params.extraRenderStages.begin(); rsItr != params.extraRenderStages.end(); ++rsItr )
    {
        frHelper << *rsItr;
    }
    frHelper.commit();
    frFactory->appendStage( new MPRStage( params.geometryTypePlanes ) );
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
