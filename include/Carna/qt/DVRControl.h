/*
 *  Copyright (C) 2010 - 2014 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#ifndef DVRCONTROL_H_0874895466
#define DVRCONTROL_H_0874895466

/** \file   DVRControl.h
  * \brief  Defines \ref Carna::qt::DVRControl
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/qt/RenderStageControl.h>
#include <Carna/base/noncopyable.h>
#include <QWidget>

class QDoubleSpinBox;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DVRControl
// ----------------------------------------------------------------------------------

class CARNAQT_LIB DVRControl : public QWidget, public RenderStageControl
{

    Q_OBJECT
    NON_COPYABLE

public:

    /** \brief
      * Instantiates.
      */
    explicit DVRControl( presets::DVRStage&, QWidget* parent = nullptr );

    /** \brief
      * Deletes.
      */
    virtual ~DVRControl();

    presets::DVRStage& dvr;
    
public slots:

    void setTranslucence( double translucence );

private:

    QDoubleSpinBox* const sbTranslucence;

    QDoubleSpinBox* const sbDiffuseLight;

    QDoubleSpinBox* const sbAmbientLight;

    ColorMapEditor* const colorMapEditor;

private slots:

    /** \brief
      * Prompts the user for an XML output file and saves current channels there.
      */
    void saveColorConfig();

    /** \brief
      * Prompts the user for an XML input file and loads channels from there.
      */
    void loadColorConfig();

    void updateColorMap();

}; // DVRControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // DVRCONTROL_H_0874895466
