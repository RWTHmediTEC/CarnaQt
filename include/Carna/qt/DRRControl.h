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
#include <Carna/qt/RenderStageControl.h>
#include <QWidget>

class QDoubleSpinBox;
class QSpinBox;

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
  * \author Leonid Kostrykin
  * \date   16.4.12 - 2.4.15
  */
class CARNAQT_LIB DRRControl : public QWidget, public RenderStageControl
{

    Q_OBJECT

public:

    /** \brief
      * Instantiates.
      */
    explicit DRRControl( presets::DRRStage& drr, QWidget* parent = nullptr );
    
    presets::DRRStage& drr;

public slots:

    void setWaterAttenuation( double muWater );

    void setBaseIntensity( double baseIntensity );

    void setLowerThreshold( int lowerThreshold );

    void setUpperThreshold( int upperThreshold );

    void setUpperMultiplier( double upperMultiplier );

private:

    QDoubleSpinBox* const sbWaterAttenuation;

    QDoubleSpinBox* const sbBrightness;

    QSpinBox* const sbLowerThreshold;

    QSpinBox* const sbUpperThreshold;

    QDoubleSpinBox* const sbUpperMultiplier;

}; // DRRControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // DRRCONTROL_H_0874895466
