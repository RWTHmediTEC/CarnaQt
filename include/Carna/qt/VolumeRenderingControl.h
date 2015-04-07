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

    presets::VolumeRenderingStage& stage;
    
public slots:

    void setSampleRate( int samplesPerPixel );

protected:

    QSpinBox* const sbSampleRate;

}; // VolumeRenderingControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // VOLUMERENDERINGCONTROL_H_0874895466
