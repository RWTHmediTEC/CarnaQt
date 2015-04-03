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

#ifndef COLORPICKERPAINTER_H_0874895466
#define COLORPICKERPAINTER_H_0874895466

/** \file   ColorPickerPainter.h
  * \brief  Defines \ref Carna::qt::ColorPickerPainter.
  */

#include <Carna/qt/CarnaQt.h>

class QPainter;
class QColor;
class QRect;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorPickerPainter
// ----------------------------------------------------------------------------------

/** \brief
  * Utilized by \ref ColorPicker and \ref WideColorPicker to paint visual
  * representations for colors and alpha values.
  *
  * Examples:
  *
  * \image html ColorPicker01.png
  *
  * \author Leonid Kostrykin
  * \date   28.7.11 - 2.4.15
  */
class CARNAQT_LIB ColorPickerPainter
{

public:

    /** \brief
      * Paints box of given size, filled with given color.
      */
    void paint( QPainter& painter
              , const QRect& rect
              , const QColor& color
              , bool ignoreAlpha = false );

}; // ColorPickerPainter



}  // namespace Carna :: qt

}  // namespace Carna

#endif // COLORPICKERPAINTER_H_0874895466
