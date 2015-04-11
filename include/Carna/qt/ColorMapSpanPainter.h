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

#ifndef COLORMAPSPANPAINTER_H_0874895466
#define COLORMAPSPANPAINTER_H_0874895466

/** \file   ColorMapSpanPainter.h
  * \brief  Defines \ref Carna::qt::ColorMapSpanPainter.
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/qt/IntSpanPainter.h>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorMapSpanPainter
// ----------------------------------------------------------------------------------

/** \brief
  * Implements \ref IntSpanPainter s.t. it visualizes color gradients.
  *
  * The painter needs to be associated with a ColorMapEditor by the invocation of
  * \ref ColorMapSpanPainter::setWidget to work properly.
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 2.4.15
  */
class CARNAQT_LIB ColorMapSpanPainter : public IntSpanPainter
{

public:

    virtual void paint( QPainter&, const base::math::Span< int >&, int span_index );

    /** \brief
      * Associates the painter with some ColorMapEditor.
      */
    void setWidget( ColorMapEditor& widget )
    {
        this->widget = &widget;
    }

private:

    /** \brief
      * References the widget which this painter is associated with.
      */
    ColorMapEditor* widget;

}; // ColorMapSpanPainter



}  // namespace Carna :: qt

}  // namespace Carna

#endif // COLORMAPSPANPAINTER_H_0874895466
