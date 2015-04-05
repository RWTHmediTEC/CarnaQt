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

#include <Carna/qt/SpatialListModel.h>
#include <Carna/qt/SpatialListModelDetails.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Node.h>
#include <Carna/base/Spatial.h>
#include <QTimer>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// SpatialListModel :: Details
// ----------------------------------------------------------------------------------

SpatialListModel::Details::Details( SpatialListModel& self )
    : self( self )
    , spatialMapper( nullptr )
    , root( nullptr )
    , invalidated( false )
{
}


void SpatialListModel::Details::removeRootListener()
{
    if( root != nullptr )
    {
        root->removeNodeListener( *this );
    }
}


void SpatialListModel::Details::onNodeDelete( const base::Node& node )
{
    CARNA_ASSERT( &node == root );
    
    /* We do not use 'removeRootListener' here because we are not allowed to remove
     * the listener from that dying node.
     */
    root = nullptr;
    
    /* Invalidate explicitly here because 'onTreeChange' will not get notified.
     */
    invalidate();
}


void SpatialListModel::Details::onTreeChange( base::Node& node, bool inThisSubtree )
{
    if( inThisSubtree )
    {
        invalidate();
    }
}


void SpatialListModel::Details::onTreeInvalidated( base::Node& subtree )
{
}


void SpatialListModel::Details::invalidate()
{
    if( !invalidated )
    {
        invalidated = true;
        QTimer::singleShot( 0, this, SLOT( update() ) );
    }
}


void SpatialListModel::Details::update()
{
    invalidated = false;
    const std::size_t previousCount = spatials.size();
    spatials.clear();
    if( root != nullptr )
    {
        CARNA_ASSERT( spatialMapper != nullptr );
        const SpatialMapper& map = *spatialMapper;

        /* Gather objects avoiding duplicates.
         */
        std::set< base::Spatial* > set;
        root->visitChildren( true, [&map, &set]( base::Spatial& spatial )
            {
                base::Spatial* const mapped = map( spatial );
                if( mapped != nullptr )
                {
                    set.insert( mapped );
                }
            }
        );
        
        /* Finish the update.
         */
        spatials = std::vector< base::Spatial* >( set.begin(), set.end() );
        const std::size_t affectedRows = std::max( previousCount, spatials.size() );
        emit self.dataChanged( self.index( 0, 0 ), self.index( affectedRows - 1, 0 ) );
    }
}



// ----------------------------------------------------------------------------------
// pickMovableSpatial
// ----------------------------------------------------------------------------------

static base::Spatial* pickMovableSpatial( base::Spatial& spatial )
{
    if( spatial.isMovable() )
    {
        return &spatial;
    }
    else
    if( spatial.hasParent() )
    {
        return pickMovableSpatial( spatial.parent() );
    }
    else
    {
        return nullptr;
    }
}



// ----------------------------------------------------------------------------------
// pickMovableGeometry
// ----------------------------------------------------------------------------------

static base::Spatial* pickMovableGeometry( base::Spatial& spatial )
{
    if( dynamic_cast< base::Geometry* >( &spatial ) != nullptr )
    {
        return pickMovableSpatial( spatial );
    }
    else
    {
        return nullptr;
    }
}



// ----------------------------------------------------------------------------------
// SpatialListModel
// ----------------------------------------------------------------------------------

const SpatialListModel::SpatialMapper SpatialListModel::PICK_MOVABLE_GEOMETRY = &pickMovableGeometry;


SpatialListModel::SpatialListModel( const SpatialMapper& spatialMapper )
    : pimpl( new Details( *this ) )
{
    setSpatialMapper( spatialMapper );
}


SpatialListModel::SpatialListModel( base::Node& root, const SpatialMapper& spatialMapper )
    : pimpl( new Details( *this ) )
{
    setSpatialMapper( spatialMapper );
    setRoot( root );
}


SpatialListModel::~SpatialListModel()
{
    pimpl->removeRootListener();
}


void SpatialListModel::setSpatialMapper( const SpatialMapper& spatialMapper )
{
    if( pimpl->spatialMapper != &spatialMapper )
    {
        pimpl->spatialMapper = &spatialMapper;
        pimpl->invalidate();
    }
}


void SpatialListModel::setRoot( base::Node& root )
{
    if( pimpl->root != &root )
    {
        pimpl->removeRootListener();
        pimpl->root = &root;
        root.addNodeListener( *pimpl );
        pimpl->invalidate();
    }
}


std::size_t SpatialListModel::spatials() const
{
    return pimpl->spatials.size();
}


base::Spatial& SpatialListModel::spatialAt( std::size_t index )
{
    CARNA_ASSERT( index < spatials() );
    return *pimpl->spatials[ index ];
}


const base::Spatial& SpatialListModel::spatialAt( std::size_t index ) const
{
    CARNA_ASSERT( index < spatials() );
    return *pimpl->spatials[ index ];
}


int SpatialListModel::rowCount( const QModelIndex& parent ) const
{
    return spatials();
}


QVariant SpatialListModel::data( const QModelIndex& index, int role ) const
{
    const base::Spatial& spatial = spatialAt( index.row() );
    switch( role )
    {
    
    case Qt::DisplayRole:
        return spatial.tag().empty() ? QString( "Unknown Object" ) : QString::fromStdString( spatial.tag() );
    
    /*
    case Qt::DecorationRole:
        return QIcon( "" );
        */
        
    default:
        return QVariant();
    
    }
}



}  // namespace Carna :: qt

}  // namespace Carna

