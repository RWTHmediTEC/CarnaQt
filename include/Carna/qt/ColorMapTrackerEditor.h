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

#ifndef COLORMAPTRACKEREDITOR_H_0874895466
#define COLORMAPTRACKEREDITOR_H_0874895466

/** \file   ColorMapTrackerEditor.h
  * \brief  Defines \ref Carna::VolumeRenderings::DVR::ColorMapTrackerEditor.
  */

#include <Carna/qt/CarnaQt.h>
#include <QWidget>

class QSpinBox;
class QColor;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorMapTrackerEditor
// ----------------------------------------------------------------------------------

/** \brief
  * Implements user interface for \ref ColorMapTracker configuration.
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 2.4.15
  */
class CARNAQT_LIB ColorMapTrackerEditor : public QWidget
{

    Q_OBJECT

public:

    /** \brief
      * Instantiates.
      */
    ColorMapTrackerEditor( ColorMapEditor& widget
                         , unsigned int color_index
                         , int current_huv
                         , QWidget* parent = 0 );

private:

    /** \brief
      * References the widget which the edited trackers belong to.
      */
    ColorMapEditor& widget;

    /** \brief
      * Identifies the edited tracker by the index of its assigned color.
      */
    const unsigned int colorIndex;

    /** \brief
      * Holds whether the current settings made by user are valid.
      */
    bool validState;

    /** \brief
      * References HUV editing UI element.
      */
    QSpinBox* const sbHuv;


public slots:

    /** \brief
      * Sets current HUV setting for the editor.
      *
      * The associated tracker will be updated immediately if the HUV setting is
      * valid. In this case, also \ref ColorMapTrackerEditor::validState will be set
      * to `true`. Otherwise it will be set to `false`.
      */
    void setCurrentHuv( int huv );

protected:

    /** \brief
      * Paints the widget.
      *
      * If \ref ColorMapTrackerEditor::validState is set to `false`, a red background
      * will be painted.
      */
    virtual void paintEvent( QPaintEvent* );

private slots:

    /** \brief
      * Updates internal state.
      */
    void updateColor( const QColor& );

    /** \brief
      * Updates internal state.
      */
    void updateHuv( int newHuv );

}; // ColorMapTrackerEditor



}  // namespace Carna :: qt

}  // namespace Carna

#endif // COLORMAPTRACKEREDITOR_H_0874895466
