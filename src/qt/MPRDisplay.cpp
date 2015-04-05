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
#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/presets/OrthogonalControl.h>
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
// NullCameraControl
// ----------------------------------------------------------------------------------

class NullCameraControl : public base::CameraControl
{

public:

    virtual void setCamera( base::Spatial& camera ) override;
    virtual void rotateHorizontally( float radians ) override;
    virtual void rotateVertically( float radians ) override;
    virtual void moveAxially( float distance ) override;

}; // NullCameraControl


void NullCameraControl::setCamera( base::Spatial& camera )
{
}


void NullCameraControl::rotateHorizontally( float radians )
{
}


void NullCameraControl::rotateVertically( float radians )
{
}


void NullCameraControl::moveAxially( float distance )
{
}



// ----------------------------------------------------------------------------------
// MPRDisplay :: Details
// ----------------------------------------------------------------------------------

struct MPRDisplay::Details : public base::NodeListener, public QObject
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    
    MPRDisplay& self;
    Details( MPRDisplay& self, const Parameters& params );

    Display* const display;
    static Display* createDisplay( const Parameters& params );

    base::Node* root;
    base::Spatial* volume;
    void findVolume();
    
    base::math::Matrix4f pivotRotation;
    base::Node pivot;
    base::Geometry* const plane;
    base::Camera* const cam;
    const std::unique_ptr< presets::OrthogonalControl > projControl;
    void updatePivot();
    
    virtual void onNodeDelete( const base::Node& node ) override;
    virtual void onTreeChange( base::Node& node, bool inThisSubtree ) override;
    virtual void onTreeInvalidated( base::Node& subtree ) override;
    
    virtual bool eventFilter( QObject* obj, QEvent* ev ) override;
};


MPRDisplay::Details::Details( MPRDisplay& self, const Parameters& params )
    : self( self )
    , display( createDisplay( params ) )
    , root( nullptr )
    , volume( nullptr )
    , plane( new base::Geometry( params.geometryTypePlanes ) )
    , cam( new base::Camera() )
    , projControl( new presets::OrthogonalControl( new NullCameraControl() ) )
    , pivotRotation( base::math::identity4f() )
{
    /* Configure the 'pivot' node.
     */
    pivot.attachChild( plane );
    pivot.attachChild( cam );
    updatePivot();
    
    /* Configure the camera.
     */
    projControl->setZoomStrength( 1e-2f );
    cam->localTransform = base::math::translation4f( 0, 0, params.visibleDistance / 2 );
    projControl->setMinimumVisibleDistance( 0 );
    projControl->setMaximumVisibleDistance( params.visibleDistance );
    
    /* Configure the display.
     */
    display->setCamera( *cam );
    display->setCameraControl( new base::Aggregation< base::CameraControl >( *projControl ) );
    display->setProjectionControl( new base::Aggregation< base::ProjectionControl >( *projControl ) );
    display->installEventFilter( this );
}


bool MPRDisplay::Details::eventFilter( QObject* obj, QEvent* ev )
{
    if( ev->type() == QEvent::Wheel )
    {
        display->updateProjection();
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
    Display* const display = new Display( frFactory );
    return display;
}


void MPRDisplay::Details::findVolume()
{
    /* Our goal is to find all volume nodes within the scene. We must report an error
     * if there is more than one, because this is not supported for the moment. The
     * ad-hoc solution is to use distinct 'MPRDisplay' instances for each volume.
     * First we look for all 'Geometry' leafs that have proper 'geometryType'.
     */
    const unsigned int volumetric = self.geometryTypeVolume;
    std::vector< base::Spatial* > seeds;
    root->visitChildren( true, [volumetric, &seeds]( base::Spatial& spatial )
        {
            base::Geometry* const geometry = dynamic_cast< base::Geometry* >( &spatial );
            if( geometry != nullptr && geometry->geometryType == volumetric )
            {
                seeds.push_back( geometry );
            }
        }
    );
    
    /* We are done here if no 'seeds' have been found.
     */
    if( seeds.empty() )
    {
        volume = nullptr;
        return;
    }
    
    /* Merge the 'seeds' to their parent nodes successively.
     */
    while( seeds.size() > 1 )
    {
        /* Merge the 'seeds' to their parents. Verify that all remained seeds are not
         * movable. If this is not true, than we have encountered a scene with more
         * than one volume.
         */
        std::set< base::Spatial* > parents;
        for( auto seedItr = seeds.begin(); seedItr != seeds.end(); ++seedItr )
        {
            base::Spatial& spatial = **seedItr;
            CARNA_ASSERT_EX
                ( !spatial.isMovable() || !spatial.hasParent()
                , "MPRDemo does not support scenes with more than one volume data nodes!" );
                
            parents.insert( &spatial.parent() );
        }
        
        /* Use the 'parents' found as the new 'seeds'.
         */
        seeds = std::vector< base::Spatial* >( parents.begin(), parents.end() );
    }
    
    /* We have found the one volume node we have been looking for.
     */
    volume = seeds[ 0 ];
    updatePivot();
}


void MPRDisplay::Details::onNodeDelete( const base::Node& node )
{
    CARNA_ASSERT( &node == root );
    root = nullptr;
    pivot.detachFromParent();
    self.invalidate();
}


void MPRDisplay::Details::onTreeChange( base::Node& node, bool inThisSubtree )
{
    if( inThisSubtree )
    {
        findVolume();
    }
}


void MPRDisplay::Details::onTreeInvalidated( base::Node& subtree )
{
    updatePivot();
}


void MPRDisplay::Details::updatePivot()
{
    if( volume != nullptr )
    {
        CARNA_ASSERT( volume->hasParent() );
        
        /* Update the 'pivot' transform s.t. it matches the parent of the volume,
         * i.e. rotation and translation become the same, but the volume is still
         * scaled w.r.t. to the pivot.
         */
        pivot.localTransform = pivotRotation;
        for( base::Spatial* current = &volume->parent(); current != root; current = &current->parent() )
        {
            pivot.localTransform = current->localTransform * pivot.localTransform;
        }
    }
}



// ----------------------------------------------------------------------------------
// MPRDisplay :: Parameters
// ----------------------------------------------------------------------------------

MPRDisplay::Parameters::Parameters()
    : geometryTypeVolume( DEFAULT_GEOMETRY_TYPE_VOLUME )
    , geometryTypePlanes( DEFAULT_GEOMETRY_TYPE_PLANES )
    , unzoomedVisibleSideLength( DEFAULT_UNZOOMED_VISIBLE_SIDE_LENGTH )
    , visibleDistance( DEFAULT_VISIBLE_DISTANCE )
{
}



// ----------------------------------------------------------------------------------
// MPRDisplay
// ----------------------------------------------------------------------------------

const float MPRDisplay::DEFAULT_UNZOOMED_VISIBLE_SIDE_LENGTH = 500;
const float MPRDisplay::DEFAULT_VISIBLE_DISTANCE = 2000;


MPRDisplay::MPRDisplay( const Parameters& params, QWidget* parent )
    : QWidget( parent )
    , pimpl( new Details( *this, params ) )
    , geometryTypeVolume( params.geometryTypeVolume )
    , geometryTypePlanes( params.geometryTypePlanes )
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
    pimpl->pivot.detachFromParent();
    pimpl->display->removeEventFilter( pimpl.get() );
}


QSize MPRDisplay::minimumSizeHint() const
{
    return QSize( 400, 400 );
}


void MPRDisplay::setRoot( base::Node& root )
{
    if( pimpl->root != &root )
    {
        if( pimpl->root != nullptr )
        {
            pimpl->root->removeNodeListener( *pimpl );
            pimpl->pivot.detachFromParent();
        }
        pimpl->root = &root;
        pimpl->root->addNodeListener( *pimpl );
        pimpl->root->attachChild( &pimpl->pivot );
        pimpl->findVolume();
        invalidate();
    }
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



}  // namespace Carna :: qt

}  // namespace Carna

