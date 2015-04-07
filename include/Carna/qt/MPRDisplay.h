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
  * Implements interactive *multiplanar reformat* widget that connects to a scene
  * through an \ref MPR object.
  *
  * Each `%MPRDisplay` instance maintains a dedicated scene graph node. This *pivot*
  * node holds the two scene graph leafs that each `%MPRDisplay` requires, namely its
  * `base::Camera` and an `base::Geometry` instance that represents the cutting plane
  * whose normal vector is parallel to the viewing direction. This cutting plane is
  * shortly referred to as "the plane" within this class.
  *
  * An `%MPRDisplay` object setups its camera to use an orthogonal projection. It
  * depends on the scalings within the scene graph and the units of the volumetric
  * data, but in a usual setup a pixel corresponds to \f$1/f\f$ millimeters at zoom
  * factor \f$f\f$. The \ref zoomFactor method queries the current zoom factor.
  *
  * \author Leonid Kostrykin
  * \date   5.4.15 - 7.4.15
  */
class CARNAQT_LIB MPRDisplay : public QWidget
{

    NON_COPYABLE
    
    struct Details;
    std::unique_ptr< Details > pimpl;

public:
    
    const static float DEFAULT_VISIBLE_DISTANCE;
    const static base::Color DEFAULT_PLANE_COLOR;
    
    const static base::math::Matrix3f ROTATION_LEFT;
    const static base::math::Matrix3f ROTATION_TOP;
    
    /** \brief
      * Specifies the configuration parameters of a \ref MPRDisplay.
      */
    struct Parameters
    {
        /** \brief
          * Sets \ref visibleDistance to \ref DEFAULT_VISIBLE_DISTANCE.
          */
        Parameters( unsigned int geometryTypeVolume, unsigned int geometryTypePlanes );
        
        unsigned int geometryTypeVolume;
        unsigned int geometryTypePlanes;
        float visibleDistance;
    };
    
    /** \brief
      * Configures a \ref MPRDisplay actively by supplying additional rendering
      * stages.
      */
    struct Configurator
    {
        Configurator( const Parameters& params );
        virtual ~Configurator();
        const Parameters& parameters;
        virtual void addExtraStages( base::RenderStageSequence& to ) const;
    };

    /** \brief
      * Instantiates.
      */
    explicit MPRDisplay( const Configurator& cfg, QWidget* parent = nullptr );

    /** \brief
      * Invokes \ref removeFromMPR and deletes.
      */
    virtual ~MPRDisplay();

    /** \brief
      * Holds the configuration of this `%MPRDisplay`.
      */
    const Parameters parameters;
    
    /** \brief
      * Tells the recommended minimum size of this widget.
      */
    virtual QSize minimumSizeHint() const override;
    
    /** \brief
      * Sets the rotation this display uses w.r.t. the local coordinate system of the
      * volumetric grid's parent node.
      */
    void setRotation( const base::math::Matrix3f& rotation );
    
    /** \brief
      * Denotes that the display should update its rendering.
      *
      * An appropriate event is posted to the Qt message queue.
      */
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
    
    /** \brief
      * Attaches the node, that holds camera and cutting plane, \a toParent.
      */
    void attachPivot( base::Node& toParent );
    
    /** \brief
      * Detaches the node, that holds camera and cutting plane, from its parent.
      */
    void detachPivot();
    
    /** \brief
      * Updates the `base::Spatial::localTransform` of the *pivot* node, that holds
      * camera and cutting plane.
      *
      * \param baseTransform transforms from the parent space of the volumetric grid
      *     to the parent space of the *pivot* node, i.e. the world space w.r.t. the
      *     root node of the `%MPR` this display is attached to.
      *
      * The method sets pivot's `base::Spatial::localTransform` s.t. its local space
      * matches the space of the grid, but \ref setRotation "rotated" occasionally.
      */
    void updatePivot( const base::math::Matrix4f& baseTransform );
    
    /** \brief
      * Attaches this `%MPRDisplay` to \a mpr. This is equivalent to
      * \ref MPR::addDisplay.
      *
      * If the this `%MPRDisplay` is attached to a different `%MPR`, it is
      * \ref removeFromMPR "removed" from that first.
      */
    void setMPR( MPR& mpr );
    
    /** \brief
      * Removes this `%MPRDisplay` from the `%MPR` it is attached to. This is
      * equivalent to \ref MPR::removeDisplay.
      *
      * Nothing happens if this `%MPRDisplay` is not attached to any `%MPR`.
      */
    void removeFromMPR();
    
    /** \brief
      * Sets the illustration color the cutting plane from this `%MPRDisplay` within
      * other displays.
      */
    void setPlaneColor( const base::Color& color );
    
    /** \brief
      * Tells the illustration color the cutting plane from this `%MPRDisplay` within
      * other displays.
      */
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
    
    /** \brief
      * Tells the current zoom factor.
      */
    float zoomFactor() const;

}; // MPRDisplay



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MPRDISPLAY_H_0874895466

