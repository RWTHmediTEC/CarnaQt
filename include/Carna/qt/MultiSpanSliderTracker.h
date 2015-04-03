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

#ifndef MULTISPANSLIDERTRACKER_H_0874895466
#define MULTISPANSLIDERTRACKER_H_0874895466

/** \file   MultiSpanSliderTracker.h
  * \brief  Defines \ref Carna::qt::MultiSpanSliderTracker.
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/math/Span.h>
#include <Qt>

class QPoint;
class QPainter;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MultiSpanSliderTracker
// ----------------------------------------------------------------------------------

/** \brief
  * Defines a \ref MultiSpanSlider tracker.
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 2.4.15
  */
class CARNAQT_LIB MultiSpanSliderTracker
{

public:

    /** \brief
      * Deletes.
      */
    virtual ~MultiSpanSliderTracker();


    /** \brief
      * Each tracker marks either the beginning or the ending of an interval.
      */
    enum Role
    {
        begin,  ///< Denotes that a tracker marks the beginning of an interval.
        end     ///< Denotes that a tracker marks the    ending of an interval.
    };

    /** \brief
      * Paints the tracker with given parameters.
      *
      * \param span
      *     references the span, this tracker belongs to.
      *
      * \param spanIndex
      *     tells what number \a span is in the context of the \ref MultiSpanSlider
      *     it is associated to.
      *
      * \param orientation
      *     defines whether the tracker is moving horizontally or vertically.
      *
      * \param painter
      *     is the `QPainter` to use for painting.
      *
      * \param mode
      *     tells whether the tracker to be drawn marks the beginning or the ending
      *     of the references span.
      */
    virtual void paint( const base::math::Span< int >& span
                      , unsigned int spanIndex
                      , Qt::Orientation orientation
                      , QPainter& painter
                      , Role mode );

    /** \brief
      * Tests, whether the tracker is hit by given point.
      *
      * The test does not have to be accurate.
      *
      * \param point
      *     is the point which is to be tested.
      *
      * \param orientation
      *     defines whether the tracker is moving horizontally or vertically.
      *
      * \param mode
      *     tells whether the tracker being tested marks the beginning or the ending
      *     of it's associated span.
      */
    virtual bool testHit( const QPoint& point
                        , Qt::Orientation orientation
                        , Role mode );

    /** \brief
      * Holds the width of the tracker as painted by
      * \ref MultiSpanSliderTracker::paint and assumed by
      * \ref MultiSpanSliderTracker::testHit.
      */
    static const int trackerWidth;

    /** \brief
      * Holds the height of the tracker as painted by
      * \ref MultiSpanSliderTracker::paint and assumed by
      * \ref MultiSpanSliderTracker::testHit.
      */
    static const int trackerHeight;

}; // MultiSpanSliderTracker



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MULTISPANSLIDERTRACKER_H_0874895466
