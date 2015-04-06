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
  *  -# Start with including headers from base %Carna and this packge:
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
class CARNAQT_LIB DRRControl : protected VolumeRenderingControl
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
    
    presets::DRRStage& drr;

public slots:

    void setWaterAttenuation( double muWater );

    void setBaseIntensity( double baseIntensity );

    void setLowerThreshold( int lowerThreshold );

    void setUpperThreshold( int upperThreshold );

    void setUpperMultiplier( double upperMultiplier );
    
    void setRenderingInverse( int renderingInverse );

}; // DRRControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // DRRCONTROL_H_0874895466
