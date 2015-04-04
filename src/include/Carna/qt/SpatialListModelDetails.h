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

#ifndef SPATIALLISTMODELDETAILS_H_0874895466
#define SPATIALLISTMODELDETAILS_H_0874895466

/** \file   SpatialListModelDetails.h
  * \brief  Defines implementation details of \ref Carna::qt::SpatialListModel.
  */

#include <Carna/qt/SpatialListModel.h>
#include <Carna/base/NodeListener.h>
#include <QObject>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// SpatialListModel :: Details
// ----------------------------------------------------------------------------------

class SpatialListModel::Details : public QObject, public base::NodeListener
{

    Q_OBJECT

public:

    Details( SpatialListModel& self );
    SpatialListModel& self;
    
    const SpatialMapper* spatialMapper;
    
    base::Node* root;
    void removeRootListener();
    
    virtual void onNodeDelete( const base::Node& node ) override;
    virtual void onTreeChange( base::Node& node, bool inThisSubtree ) override;
    virtual void onTreeInvalidated( base::Node& subtree ) override;
    
    bool invalidated;
    std::vector< base::Spatial* > spatials;
    void invalidate();
    
public slots:

    void update();
    
}; // SpatialListModel :: Details



}  // namespace Carna :: qt

}  // namespace Carna

#endif // SPATIALLISTMODELDETAILS_H_0874895466
