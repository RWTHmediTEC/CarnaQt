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

#include <Carna/qt/MPR.h>
#include <Carna/base/Node.h>
#include <Carna/base/NodeListener.h>
#include <set>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPR :: Details
// ----------------------------------------------------------------------------------

struct MPR::Details : public base::NodeListener
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    
    MPR& self;
    Details( MPR& self );

    base::Node* root;
    base::Spatial* volume;
    void findVolume();
    
    void attachPivots();
    void detachPivots();
    void updatePivots();
    base::math::Matrix4f basePivotTransform;
    
    std::set< MPRDisplay* > displays;
    
    virtual void onNodeDelete( const base::Node& node ) override;
    virtual void onTreeChange( base::Node& node, bool inThisSubtree ) override;
    virtual void onTreeInvalidated( base::Node& subtree ) override;
};


MPR::Details::Details( MPR& self )
    : self( self )
    , root( nullptr )
    , volume( nullptr )
{
}


void MPR::Details::findVolume()
{
    /* Our goal is to find all volume nodes within the scene. We must report an error
     * if there is more than one, because this is not supported for the moment. The
     * ad-hoc solution is to use distinct 'MPR' instances for each volume. First we
     * look for all 'Geometry' leafs that have proper 'geometryType'.
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
                , "MPR does not support scenes with more than one volume data nodes!" );
                
            parents.insert( &spatial.parent() );
        }
        
        /* Use the 'parents' found as the new 'seeds'.
         */
        seeds = std::vector< base::Spatial* >( parents.begin(), parents.end() );
    }
    
    /* We have found the one volume node we have been looking for.
     */
    volume = seeds[ 0 ];
    updatePivots();
}


void MPR::Details::attachPivots()
{
    CARNA_ASSERT( root != nullptr );
    for( auto displayItr = displays.begin(); displayItr != displays.end(); ++displayItr )
    {
        MPRDisplay& display = **displayItr;
        display.attachPivot( *root );
    }
}


void MPR::Details::detachPivots()
{
    for( auto displayItr = displays.begin(); displayItr != displays.end(); ++displayItr )
    {
        MPRDisplay& display = **displayItr;
        display.detachPivot();
    }
}


void MPR::Details::updatePivots()
{
    if( volume != nullptr )
    {
        CARNA_ASSERT( volume->hasParent() );
        
        /* Update the 'pivot' transform s.t. it matches the parent of the volume,
         * i.e. rotation and translation become the same, but the volume is still
         * scaled w.r.t. to the pivot.
         */
        basePivotTransform = base::math::identity4f();
        for( base::Spatial* current = &volume->parent(); current != root; current = &current->parent() )
        {
            basePivotTransform = current->localTransform * basePivotTransform;
        }
        
        /* Notify all displays to update their pivots.
         */
        for( auto displayItr = displays.begin(); displayItr != displays.end(); ++displayItr )
        {
            MPRDisplay& display = **displayItr;
            display.updatePivot( basePivotTransform );
        }
    }
}


void MPR::Details::onNodeDelete( const base::Node& node )
{
    CARNA_ASSERT( &node == root );
    root = nullptr;
    detachPivots();
}


void MPR::Details::onTreeChange( base::Node& node, bool inThisSubtree )
{
    if( inThisSubtree )
    {
        findVolume();
    }
}


void MPR::Details::onTreeInvalidated( base::Node& subtree )
{
    updatePivots();
}



// ----------------------------------------------------------------------------------
// MPR
// ----------------------------------------------------------------------------------

MPR::MPR( unsigned int geometryTypeVolume )
    : pimpl( new Details( *this ) )
    , geometryTypeVolume( geometryTypeVolume )
{
}


MPR::~MPR()
{
    const std::vector< MPRDisplay* > displays( pimpl->displays.begin(), pimpl->displays.end() );
    for( auto displayItr = displays.begin(); displayItr != displays.end(); ++displayItr )
    {
        removeDisplay( **displayItr );
    }
}


void MPR::addDisplay( MPRDisplay& mprDisplay )
{
    const std::size_t originalDisplaysCount = pimpl->displays.size();
    pimpl->displays.insert( &mprDisplay );
    if( originalDisplaysCount != pimpl->displays.size() )
    {
        mprDisplay.setMPR( *this );
        if( pimpl->root != nullptr )
        {
            mprDisplay.attachPivot( *pimpl->root );
            if( pimpl->volume != nullptr )
            {
                mprDisplay.updatePivot( pimpl->basePivotTransform );
            }
        }
    }
}


void MPR::removeDisplay( MPRDisplay& mprDisplay )
{
    const std::size_t originalDisplaysCount = pimpl->displays.size();
    pimpl->displays.erase( &mprDisplay );
    if( originalDisplaysCount != pimpl->displays.size() )
    {
        mprDisplay.detachPivot();
        mprDisplay.removeFromMPR();
    }
}


void MPR::setRoot( base::Node& root )
{
    if( pimpl->root != &root )
    {
        if( pimpl->root != nullptr )
        {
            pimpl->root->removeNodeListener( *pimpl );
            pimpl->detachPivots();
        }
        pimpl->root = &root;
        pimpl->root->addNodeListener( *pimpl );
        pimpl->attachPivots();
        pimpl->findVolume();
    }
}


bool MPR::hasVolume() const
{
    return pimpl->volume != nullptr;
}


base::Spatial& MPR::volume()
{
    CARNA_ASSERT( hasVolume() );
    return *pimpl->volume;
}


const base::Spatial& MPR::volume() const
{
    CARNA_ASSERT( hasVolume() );
    return *pimpl->volume;
}



}  // namespace Carna :: qt

}  // namespace Carna

