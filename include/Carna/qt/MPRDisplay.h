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

#ifndef MPRDISPLAY_H_0874895466
#define MPRDISPLAY_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/math.h>
#include <QWidget>
#include <memory>
#include <vector>

class QMouseEvent;
class QWheelEvent;

/** \file   MPRDisplay.h
  * \brief  Defines \ref Carna::qt::MPRDisplay.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPRDisplay
// ----------------------------------------------------------------------------------

/** \brief
  * Implements interactive *multiplanar reformat* widget.
  *
  * \author Leonid Kostrykin
  * \date   5.4.15
  */
class CARNAQT_LIB MPRDisplay : public QWidget
{

    NON_COPYABLE
    
    struct Details;
    std::unique_ptr< Details > pimpl;

public:

    const static unsigned int DEFAULT_GEOMETRY_TYPE_VOLUME = 0;
    const static unsigned int DEFAULT_GEOMETRY_TYPE_PLANES = 1;

    explicit MPRDisplay
        ( unsigned int geometryTypeVolume = DEFAULT_GEOMETRY_TYPE_VOLUME
        , unsigned int geometryTypePlanes = DEFAULT_GEOMETRY_TYPE_PLANES
        , const std::vector< base::RenderStage* >& extraRenderStages = std::vector< base::RenderStage* >()
        , QWidget* parent = nullptr );
        
    virtual ~MPRDisplay();

    const unsigned int geometryTypeVolume;
    const unsigned int geometryTypePlanes;
    
    /** \brief
      * Tells the recommended minimum size of this widget.
      */
    virtual QSize minimumSizeHint() const override;
    
    void setRoot( base::Node& root );
    
    void setRotation( const base::math::Matrix3f& rotation );
    
    void invalidate();

}; // MPRDisplay



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MPRDISPLAY_H_0874895466

