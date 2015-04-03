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

#ifndef GEOMETRYLISTMODEL_H_0874895466
#define GEOMETRYLISTMODEL_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/noncopyable.h>
#include <QAbstractListModel>
#include <memory>

class QModelIndex;
class QVariant;

/** \file   GeometryListModel.h
  * \brief  Defines \ref Carna::qt::GeometryListModel.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// GeometryListModel
// ----------------------------------------------------------------------------------

/** \brief
  * Implements `QAbstractListModel` for `base::Geometry` objects within a scene.
  *
  * \author Leonid Kostrykin
  * \date   4.4.15
  */
class CARNAQT_LIB GeometryListModel : public QAbstractListModel
{

    NON_COPYABLE
    
    class Details;
    const std::unique_ptr< Details > pimpl;

public:

    GeometryListModel();
    
    explicit GeometryListModel( base::Node& root );
    
    virtual ~GeometryListModel();
    
    void setRoot( base::Node& root );
    
    std::size_t geometries() const;
    
    base::Geometry& geometry( std::size_t index );
    
    const base::Geometry& geometry( std::size_t index ) const;
    
    virtual int rowCount( const QModelIndex& parent ) const override;
    
    virtual QVariant data( const QModelIndex& index, int role ) const override;

}; // GeometryListModel



}  // namespace Carna :: qt

}  // namespace Carna

#endif // GEOMETRYLISTMODEL_H_0874895466

