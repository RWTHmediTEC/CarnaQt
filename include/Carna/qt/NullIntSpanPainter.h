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

#ifndef NULLINTSPANPAINTER_H_0874895466
#define NULLINTSPANPAINTER_H_0874895466

/** \file   NullIntSpanPainter.h
  * \brief  Defines \ref Carna::qt::NullIntSpanPainter.
  */

#include <Carna/qt/IntSpanPainter.h>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// NullIntSpanPainter
// ----------------------------------------------------------------------------------

/** \brief
  * Null-implementation of \ref IntSpanPainter
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 2.4.15
  */
class CARNAQT_LIB NullIntSpanPainter : public IntSpanPainter
{

public:

    /** \brief
      * Does nothing.
      */
    virtual void paint( QPainter& painter, const base::math::Span< int >& span, int spanIndex ) override
    {
    }

}; // NullIntSpanPainter



}  // namespace Carna :: qt

}  // namespace Carna

#endif // NULLINTSPANPAINTER_H_0874895466
