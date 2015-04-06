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
#include <Carna/base/RenderStageSequence.h>
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
    const static base::Color DEFAULT_PLANE_COLOR;
    
    const static base::math::Matrix3f ROTATION_LEFT;
    const static base::math::Matrix3f ROTATION_TOP;
    
    struct Parameters
    {
        Parameters( unsigned int geometryTypeVolume, unsigned int geometryTypePlanes );
        
        unsigned int geometryTypeVolume;
        unsigned int geometryTypePlanes;
        float unzoomedVisibleSideLength;
        float visibleDistance;
    };
    
    struct Factory
    {
        Factory( const Parameters& params );
        virtual ~Factory();
        const Parameters& parameters;
        virtual void addExtraStages( base::RenderStageSequence& to ) const;
    };

    explicit MPRDisplay( const Factory& factory, QWidget* parent = nullptr );
        
    virtual ~MPRDisplay();

    const Parameters parameters;
    
    /** \brief
      * Tells the recommended minimum size of this widget.
      */
    virtual QSize minimumSizeHint() const override;
    
    void setRotation( const base::math::Matrix3f& rotation );
    
    void invalidate();
    
    /** \brief
      * Denotes that \a tag is to be used within logged messages to identify this
      * `%MPRDisplay` instance.
      */
    void setLogTag( const std::string& tag );
    
    /** \brief
      * Tells which tag annotates log messages to identify this `%MPRDisplay`
      * instance.
      */
    const std::string& logTag() const;
    
    void attachPivot( base::Node& to );
    
    void detachPivot();
    
    void updatePivot( const base::math::Matrix4f& baseTransform );
    
    void setMPR( MPR& mpr );
    
    void removeFromMPR();
    
    void setPlaneColor( const base::Color& color );
    
    const base::Color& planeColor() const;

    /** \brief
      * Sets windowing level to \a windowingLevel.
      */
    void setWindowingLevel( base::HUV windowingLevel );
    
    /** \brief
      * Sets windowing level to \a windowingWidth.
      */
    void setWindowingWidth( unsigned int windowingWidth );

    /** \brief
      * Tells the windowing level.
      */
    base::HUV windowingLevel() const;
    
    /** \brief
      * Tells the windowing width.
      */
    unsigned int windowingWidth() const;

    /** \brief
      * Tells the lowest HUV that is mapped to gray, but not to black or white.
      */
    base::HUV minimumHUV() const;
    
    /** \brief
      * Tells the highest HUV that is mapped to gray, but not to black or white.
      */
    base::HUV maximumHUV() const;

}; // MPRDisplay



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MPRDISPLAY_H_0874895466

