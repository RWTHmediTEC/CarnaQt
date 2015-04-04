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

#ifndef SPATIALLISTMODEL_H_0874895466
#define SPATIALLISTMODEL_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/noncopyable.h>
#include <QAbstractListModel>
#include <memory>
#include <functional>

class QModelIndex;
class QVariant;

/** \file   SpatialListModel.h
  * \brief  Defines \ref Carna::qt::SpatialListModel.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// SpatialListModel
// ----------------------------------------------------------------------------------

/** \brief
  * Implements `QAbstractListModel` for `base::Spatial` objects within a scene.
  *
  * `%SpatialListModel` processes each `base::Spatial` contained by the subtree with
  * a \ref setRoot "given root" in the following way. It maps that `base::Spatial`
  * either to itself, to another `base::Spatial` or to `nullptr`. If the result of
  * this mapping is not `nullptr`, than the resulting `base::Spatial` is added to the
  * list. The default behaviour is to gather such objects that are either movable
  * `base::Geometry` instances or the first movable parents of non-movable
  * `base::Geometry` instances. This can be changed by
  * \ref setSpatialMapper "supplying a different mapping".
  *
  * \author Leonid Kostrykin
  * \date   4.4.15
  */
class CARNAQT_LIB SpatialListModel : public QAbstractListModel
{

    NON_COPYABLE
    
    class Details;
    const std::unique_ptr< Details > pimpl;

public:

    typedef std::function< base::Spatial*( base::Spatial& ) > SpatialMapper;
    
    const static SpatialMapper PICK_MOVABLE_GEOMETRY;

    explicit SpatialListModel( const SpatialMapper& spatialMapper = PICK_MOVABLE_GEOMETRY );
    
    explicit SpatialListModel( base::Node& root, const SpatialMapper& spatialMapper = PICK_MOVABLE_GEOMETRY );
    
    virtual ~SpatialListModel();
    
    void setSpatialMapper( const SpatialMapper& spatialMapper );
    
    void setRoot( base::Node& root );
    
    std::size_t spatials() const;
    
    base::Spatial& spatialAt( std::size_t index );
    
    const base::Spatial& spatialAt( std::size_t index ) const;
    
    virtual int rowCount( const QModelIndex& parent ) const override;
    
    virtual QVariant data( const QModelIndex& index, int role ) const override;

}; // SpatialListModel



}  // namespace Carna :: qt

}  // namespace Carna

#endif // SPATIALLISTMODEL_H_0874895466

