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

#ifndef COLORMAPTRACKER_H_0874895466
#define COLORMAPTRACKER_H_0874895466

/** \file   ColorMapTracker.h
  * \brief  Defines \ref Carna::qt::ColorMapTracker.
  */

#include <Carna/qt/MultiSpanSliderTracker.h>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorMapTracker
// ----------------------------------------------------------------------------------

/** \brief
  * Specializes the \ref MultiSpanSliderTracker s.t. it also displays the color
  * assigned to the tracker within a \ref ColorMapEditor.
  *
  * The painter needs to be associated with a \ref ColorMapEditor by the invocation
  * \ref ColorMapSpanPainter::setWidget of to work properly.
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 2.4.15
  */
class CARNAQT_LIB ColorMapTracker : public MultiSpanSliderTracker
{

public:

    virtual void paint( const base::math::Span< int >&
                      , unsigned int span_index
                      , Qt::Orientation
                      , QPainter&
                      , Role );

    virtual bool testHit( const QPoint&, Qt::Orientation, Role );


    /** \brief
      * Associates the painter with some \ref ColorMapEditor.
      */
    void setWidget( ColorMapEditor& widget )
    {
        this->widget = &widget;
    }


private:

    /** \brief
      * References the widget which this tracker is associated with.
      */
    ColorMapEditor* widget;

}; // ColorMapTracker



}  // namespace Carna :: qt

}  // namespace Carna

#endif // COLORMAPTRACKER_H_0874895466
