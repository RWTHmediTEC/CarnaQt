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
#include <Carna/helpers/FrameRendererHelper.h>
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

struct MPRDisplay::Details : public base::NodeListener
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    
    MPRDisplay& self;
    Details
        ( MPRDisplay& self
        , unsigned int geometryTypeVolume
        , unsigned int geometryTypePlanes
        , const std::vector< base::RenderStage* >& extraRenderStages );

    Display* const display;
    static Display* createDisplay
        ( unsigned int geometryTypeVolume
        , unsigned int geometryTypePlanes
        , const std::vector< base::RenderStage* >& extraRenderStages );

    base::Node* root;
    base::Spatial* volume;
    void findVolume();
    
    base::math::Matrix4f pivotRotation;
    base::Node pivot;
    base::Geometry* const plane;
    base::Camera* const cam;
    void updatePivot();
    
    virtual void onNodeDelete( const base::Node& node ) override;
    virtual void onTreeChange( base::Node& node, bool inThisSubtree ) override;
    virtual void onTreeInvalidated( base::Node& subtree ) override;
};


MPRDisplay::Details::Details
        ( MPRDisplay& self
        , unsigned int geometryTypeVolume
        , unsigned int geometryTypePlanes
        , const std::vector< base::RenderStage* >& extraRenderStages )
    : self( self )
    , display( createDisplay( geometryTypeVolume, geometryTypePlanes, extraRenderStages ) )
    , root( nullptr )
    , volume( nullptr )
    , plane( new base::Geometry( geometryTypePlanes ) )
    , cam( new base::Camera() )
    , pivotRotation( base::math::identity4f() )
{
    /* Configure the 'pivot' node.
     */
    pivot.attachChild( plane );
    pivot.attachChild( cam );
    cam->setProjection( base::math::frustum4f( base::math::deg2rad( 45 ), 1, 1, 1000 ) );
    cam->localTransform = base::math::translation4f( 0, 0, 500 );
    updatePivot();
    
    /* Configure the display.
     */
    display->setCamera( *cam );
}


Display* MPRDisplay::Details::createDisplay
    ( unsigned int geometryTypeVolume
    , unsigned int geometryTypePlanes
    , const std::vector< base::RenderStage* >& extraRenderStages )
{
    FrameRendererFactory* const frFactory = new FrameRendererFactory();
    helpers::FrameRendererHelper< > frHelper( *frFactory );
    frHelper << new presets::CuttingPlanesStage( geometryTypeVolume, geometryTypePlanes );
    for( auto rsItr = extraRenderStages.begin(); rsItr != extraRenderStages.end(); ++rsItr )
    {
        frHelper << *rsItr;
    }
    frHelper.commit();
    return new Display( frFactory );
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
// MPRDisplay
// ----------------------------------------------------------------------------------

MPRDisplay::MPRDisplay
        ( unsigned int geometryTypeVolume
        , unsigned int geometryTypePlanes
        , const std::vector< base::RenderStage* >& extraRenderStages
        , QWidget* parent )
    : QWidget( parent )
    , pimpl( new Details( *this, geometryTypeVolume, geometryTypePlanes, extraRenderStages ) )
    , geometryTypeVolume( geometryTypeVolume )
    , geometryTypePlanes( geometryTypePlanes )
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

