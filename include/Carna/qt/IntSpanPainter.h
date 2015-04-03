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

#ifndef INTSPANPAINTER_H_0874895466
#define INTSPANPAINTER_H_0874895466

/** \file   IntSpanPainter.h
  * \brief  Defines \ref Carna::qt::IntSpanPainter.
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/math/Span.h>

class QPainter;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// IntSpanPainter
// ----------------------------------------------------------------------------------

/** \brief
  * Paints `base::math::Span<int>` on `QCanvas` within \ref MultiSpanSlider objects.
  *
  * Classes implementing this interface are capable of visualizing
  * `base::math::Span<int>` instances within the context of a \ref MultiSpanSlider.
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 2.4.15
  */
class CARNAQT_LIB IntSpanPainter
{

public:

    /** \brief
      * Deletes.
      */
    virtual ~IntSpanPainter()
    {
    }

    /** \brief
      * Paints \a span using \a painter.
      *
      * \param painter
      *     is the `QPainter` to use for painting.
      *
      * \param span
      *     is the span to be painted.
      *
      * \param spanIndex
      *     tells what number \a span is in the context of the \ref MultiSpanSlider
      *     it is associated to.
      */
    virtual void paint( QPainter& painter, const base::math::Span< int >& span, int spanIndex ) = 0;

}; // IntSpanPainter



}  // namespace Carna :: qt

}  // namespace Carna

#endif // INTSPANPAINTER_H_0874895466
