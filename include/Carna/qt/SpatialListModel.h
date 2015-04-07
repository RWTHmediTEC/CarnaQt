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

    /** \brief
      * Defines `base::Spatial` self-education.
      */
    typedef std::function< base::Spatial*( base::Spatial& ) > SpatialMapper;
    
    /** \brief
      * Maps `base::Spatial` that is `base::Geometry` to its first movable parent and
      * to `nullptr` otherwise.
      */
    const static SpatialMapper PICK_MOVABLE_GEOMETRY;

    /** \brief
      * Instantiates.
      */
    explicit SpatialListModel( const SpatialMapper& spatialMapper = PICK_MOVABLE_GEOMETRY );
    
    /** \overload
      */
    explicit SpatialListModel( base::Node& root, const SpatialMapper& spatialMapper = PICK_MOVABLE_GEOMETRY );
    
    /** \brief
      * Deletes.
      */
    virtual ~SpatialListModel();
    
    /** \brief
      * Specifies which `base::Spatial` object is to be enlisted for each
      * `base::Geometry` objects encountered.
      */
    void setSpatialMapper( const SpatialMapper& spatialMapper );
    
    /** \brief
      * Connects this `%SpatialListModel` to the scene \a root references. The
      * `%SpatialListModel` disconnects automatically when \a root is destroyed.
      */
    void setRoot( base::Node& root );
    
    /** \brief
      * Tells number of enlisted `base::Spatial` objects.
      */
    std::size_t spatials() const;
    
    /** \brief
      * References the `base::Spatial` object at \a index.
      */
    base::Spatial& spatialAt( std::size_t index );
    
    /** \overload
      */
    const base::Spatial& spatialAt( std::size_t index ) const;
    
    /** \brief
      * Tells number of enlisted `base::Spatial` objects.
      */
    virtual int rowCount( const QModelIndex& parent ) const override;
    
    /** \brief
      * Tells the `base::Spatial::tag` of the `base::Spatial` object at \a index.
      */
    virtual QVariant data( const QModelIndex& index, int role ) const override;

}; // SpatialListModel



}  // namespace Carna :: qt

}  // namespace Carna

#endif // SPATIALLISTMODEL_H_0874895466

