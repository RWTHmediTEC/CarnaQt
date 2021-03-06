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

#ifndef DRRCONTROL_H_0874895466
#define DRRCONTROL_H_0874895466

/** \file   DRRControl.h
  * \brief  Defines \ref Carna::qt::DRRControl.
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/qt/VolumeRenderingControl.h>
#include <memory>

class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DRRControl
// ----------------------------------------------------------------------------------

/** \brief
  * Defines `presets::%DRRStage` controlling widget.
  *
  * \image html DRRControl.png
  *
  * \section DRRControlExample Example
  *
  *  -# Start with including headers from base %Carna and this package:
  *     \snippet Demos/DRRControl/main.cpp drr_includes
  *  -# Lets stick with a single geometry type for simplicity:
  *     \snippet Demos/DRRControl/main.cpp drr_constants
  *     For explanation on the concept of geometry types, refer to the base %Carna
  *     documentation.
  *  -# Now we implement a short program:
  *     \snippet Demos/DRRControl/main.cpp drr_main
  *
  * \author Leonid Kostrykin
  * \date   16.4.12 - 2.4.15
  */
class CARNAQT_LIB DRRControl : public VolumeRenderingControl
{

    Q_OBJECT
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    explicit DRRControl( presets::DRRStage& drr, QWidget* parent = nullptr );
    
    /** \brief
      * Deletes.
      */
    virtual ~DRRControl();
    
    /** \brief
      * References the controlled rendering stage.
      */
    presets::DRRStage& drr;

public slots:

    /** \brief
      * Delegates to \ref drr and \ref RenderStageControl::invalidate "invalidates".
      */
    void setWaterAttenuation( double muWater );

    /** \brief
      * Delegates to \ref drr and \ref RenderStageControl::invalidate "invalidates".
      */
    void setBaseIntensity( double baseIntensity );

    /** \brief
      * Delegates to \ref drr and \ref RenderStageControl::invalidate "invalidates".
      */
    void setLowerThreshold( int lowerThreshold );

    /** \brief
      * Delegates to \ref drr and \ref RenderStageControl::invalidate "invalidates".
      */
    void setUpperThreshold( int upperThreshold );

    /** \brief
      * Delegates to \ref drr and \ref RenderStageControl::invalidate "invalidates".
      */
    void setUpperMultiplier( double upperMultiplier );
    
    /** \brief
      * Delegates to \ref drr and \ref RenderStageControl::invalidate "invalidates".
      */
    void setRenderingInverse( int renderingInverse );

}; // DRRControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // DRRCONTROL_H_0874895466
