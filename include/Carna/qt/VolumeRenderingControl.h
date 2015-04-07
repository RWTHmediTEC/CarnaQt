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

#ifndef VOLUMERENDERINGCONTROL_H_0874895466
#define VOLUMERENDERINGCONTROL_H_0874895466

/** \file   VolumeRenderingControl.h
  * \brief  Defines \ref Carna::qt::VolumeRenderingControl
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/qt/RenderStageControl.h>
#include <Carna/base/noncopyable.h>
#include <QWidget>

class QSpinBox;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// VolumeRenderingControl
// ----------------------------------------------------------------------------------

/** \brief
  * Base class of `presets::%VolumeRenderingStage` controlling widgets.
  *
  * Supplies a \ref sbSampleRate "widget" that controls the sample rate of the
  * controlled `presets::%VolumeRenderingStage` object. Either add this widget to a
  * layout or delete it.
  *
  * \author Leonid Kostrykin
  * \date   16.4.12 - 2.4.15
  */
class CARNAQT_LIB VolumeRenderingControl : public QWidget, protected RenderStageControl
{

    Q_OBJECT
    NON_COPYABLE

public:

    /** \brief
      * Instantiates.
      */
    explicit VolumeRenderingControl( presets::VolumeRenderingStage&, QWidget* parent = nullptr );

    /** \brief
      * Deletes.
      */
    virtual ~VolumeRenderingControl();

    /** \brief
      * References the controlled rendering stage.
      */
    presets::VolumeRenderingStage& stage;
    
public slots:

    /** \brief
      * Invokes the `setSampleRate` with \a samplesPerPixel method on the controlled
      * `presets::VolumeRenderingStage` object.
      */
    void setSampleRate( int samplesPerPixel );

protected:

    /** \brief
      * References a widget that is connected to \ref setSampleRate. Either add this
      * widget to a layout or delete it.
      */
    QSpinBox* const sbSampleRate;

}; // VolumeRenderingControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // VOLUMERENDERINGCONTROL_H_0874895466
