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
#include <Carna/qt/MPR.h>
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
    
    const static float DEFAULT_UNZOOMED_VISIBLE_SIDE_LENGTH;
    const static float DEFAULT_VISIBLE_DISTANCE;
    
    struct Parameters
    {
        Parameters( unsigned int geometryTypeVolume, unsigned int geometryTypePlanes );
        
        unsigned int geometryTypeVolume;
        unsigned int geometryTypePlanes;
        float unzoomedVisibleSideLength;
        float visibleDistance;
        
        std::vector< base::RenderStage* > extraRenderStages;
    };

    explicit MPRDisplay( const Parameters& params, QWidget* parent = nullptr );
        
    virtual ~MPRDisplay();

    const Parameters parameters;
    
    /** \brief
      * Tells the recommended minimum size of this widget.
      */
    virtual QSize minimumSizeHint() const override;
    
    void setRotation( const base::math::Matrix3f& rotation );
    
    void invalidate();
    
    void attachPivot( base::Node& to );
    
    void detachPivot();
    
    void updatePivot( const base::math::Matrix4f& baseTransform );
    
    void setMPR( MPR& mpr );
    
    void removeFromMPR();

}; // MPRDisplay



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MPRDISPLAY_H_0874895466

