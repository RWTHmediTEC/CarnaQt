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

#ifndef DVRCONTROL_H_0874895466
#define DVRCONTROL_H_0874895466

/** \file   DVRControl.h
  * \brief  Defines \ref Carna::qt::DVRControl
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/qt/VolumeRenderingControl.h>

class QDoubleSpinBox;
class QSlider;
class QShowEvent;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DVRControl
// ----------------------------------------------------------------------------------

/** \brief
  * Defines `presets::%DVRStage` controlling widget.
  *
  * \image html DVRControl.png
  *
  * \section DVRControlExample Example
  *
  *  -# Start with including headers from base %Carna and this package:
  *     \snippet Demos/DVRControl/main.cpp dvr_includes
  *  -# Lets stick with a single geometry type for simplicity:
  *     \snippet Demos/DVRControl/main.cpp dvr_constants
  *     For explanation on the concept of geometry types, refer to the base %Carna
  *     documentation.
  *  -# Now we implement a short program:
  *     \snippet Demos/DVRControl/main.cpp dvr_main
  *
  * \author Leonid Kostrykin
  * \date   2.4.15 - 3.4.15
  */
class CARNAQT_LIB DVRControl : public VolumeRenderingControl
{

    Q_OBJECT

public:

    /** \brief
      * Instantiates.
      */
    explicit DVRControl( presets::DVRStage&, QWidget* parent = nullptr );

    /** \brief
      * Deletes.
      */
    virtual ~DVRControl();

    /** \brief
      * References the controlled rendering stage.
      */
    presets::DVRStage& dvr;
    
public slots:

    /** \brief
      * Delegates to \ref dvr and \ref RenderStageControl::invalidate "invalidates".
      */
    void setTranslucence( double translucence );
    
    /** \brief
      * Delegates to \ref dvr and \ref RenderStageControl::invalidate "invalidates".
      */
    void setDiffuseLight( double diffuseLight );
    
    /** \brief
      * Updates the UI based on the `%isLightingUsed` state of \ref dvr.
      */
    void updateLightingState();

public slots:

    /** \brief
      * Prompts the user for an XML output file and saves current color map there.
      */
    void saveColorConfig();

    /** \brief
      * Prompts the user for an XML input file and loads color map from there.
      */
    void loadColorConfig();
    
protected:

    /** \brief
      * Invokes \ref updateLightingState.
      */
    virtual void showEvent( QShowEvent* event ) override;
    
    /** \brief
      * Invokes \ref updateLightingState.
      */
    virtual void onRenderingFinished();

private:

    QDoubleSpinBox* const sbTranslucence;

    QSlider* const slDiffuseLight;

    ColorMapEditor* const colorMapEditor;

private slots:

    void updateColorMap();
    
    void setDiffuseLight( int );

}; // DVRControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // DVRCONTROL_H_0874895466
