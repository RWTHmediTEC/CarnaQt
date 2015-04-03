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

#include <Carna/qt/GeometryListModel.h>
#include <Carna/qt/GeometryListModelDetails.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Node.h>
#include <QTimer>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// GeometryListModel :: Details
// ----------------------------------------------------------------------------------

GeometryListModel::Details::Details( GeometryListModel& self )
    : self( self )
    , root( nullptr )
    , invalidated( false )
{
}


void GeometryListModel::Details::removeRootListener()
{
    if( root != nullptr )
    {
        root->removeNodeListener( *this );
    }
}


void GeometryListModel::Details::onNodeDelete( const base::Node& node )
{
    CARNA_ASSERT( &node == root );
    
    /* We do not use 'removeRootListener' here because we are not allowed to remove
     * the listener from that dying node.
     */
    root = nullptr;
}


void GeometryListModel::Details::onTreeChange( base::Node& subtree )
{
    update();
}


void GeometryListModel::Details::onTreeInvalidated( base::Node& subtree )
{
}


void GeometryListModel::Details::invalidate()
{
    if( !invalidated )
    {
        invalidated = true;
        QTimer::singleShot( 0, this, SLOT( update() ) );
    }
}


void GeometryListModel::Details::update()
{
    geometries.clear();
    if( root != nullptr )
    {
        root->visitChildren( true, [&]( base::Spatial& spatial )
            {
                base::Geometry* const geometry = dynamic_cast< base::Geometry* >( &spatial );
                if( geometry != nullptr )
                {
                    geometries.push_back( geometry );
                }
            }
        );
    }
}



// ----------------------------------------------------------------------------------
// GeometryListModel
// ----------------------------------------------------------------------------------

GeometryListModel::GeometryListModel()
    : pimpl( new Details( *this ) )
{
}


GeometryListModel::GeometryListModel( base::Node& root )
    : pimpl( new Details( *this ) )
{
    setRoot( root );
}


GeometryListModel::~GeometryListModel()
{
    pimpl->removeRootListener();
}


void GeometryListModel::setRoot( base::Node& root )
{
    if( pimpl->root != &root )
    {
        pimpl->removeRootListener();
        pimpl->root = &root;
        root.addNodeListener( *pimpl );
        pimpl->invalidate();
    }
}


int GeometryListModel::rowCount( const QModelIndex& parent ) const
{
    return pimpl->geometries.size();
}


QVariant GeometryListModel::data( const QModelIndex& index, int role ) const
{
    switch( role )
    {
    
    case Qt::DisplayRole:
        return QString( "Unknown Object" );
    
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

