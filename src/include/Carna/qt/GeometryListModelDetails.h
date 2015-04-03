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

#ifndef GEOMETRYLISTMODELDETAILS_H_0874895466
#define GEOMETRYLISTMODELDETAILS_H_0874895466

/** \file   GeometryListModelDetails.h
  * \brief  Defines implementation details of \ref Carna::qt::GeometryListModel.
  */

#include <Carna/qt/GeometryListModel.h>
#include <Carna/base/NodeListener.h>
#include <QObject>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// GeometryListModel :: Details
// ----------------------------------------------------------------------------------

class GeometryListModel::Details : public QObject, public base::NodeListener
{

    Q_OBJECT

public:

    Details( GeometryListModel& self );
    GeometryListModel& self;
    
    base::Node* root;
    void removeRootListener();
    
    virtual void onNodeDelete( const base::Node& node ) override;
    virtual void onTreeChange( base::Node& node, bool inThisSubtree ) override;
    virtual void onTreeInvalidated( base::Node& subtree ) override;
    
    bool invalidated;
    std::vector< base::Geometry* > geometries;
    void update();
    
public slots:

    void invalidate();
    
}; // GeometryListModel :: Details



}  // namespace Carna :: qt

}  // namespace Carna

#endif // GEOMETRYLISTMODELDETAILS_H_0874895466
