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

#ifndef MULTISPANSLIDER_H_0874895466
#define MULTISPANSLIDER_H_0874895466

/** \file   MultiSpanSlider.h
  * \brief  Defines \ref Carna::qt::MultiSpanSlider.
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/math/Span.h>
#include <Carna/qt/NullIntSpanPainter.h>
#include <Carna/qt/MultiSpanSliderTracker.h>
#include <Carna/qt/MultiSpanSliderModelViewMapping.h>
#include <QWidget>
#include <vector>
#include <map>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MultiSpanSlider
// ----------------------------------------------------------------------------------

/** \brief
  * Defines a widget that provides an intuitive way of creating, editing and removing
  * disjoint intervals within some given boundaries through its user interface.
  *
  * \ingroup Widgets
  *
  * Example:
  *
  * \image html MultiSpanSlider01.png
  *
  * This widget can operate either vertically or horizontally. By default it operates
  * vertically.
  *
  * \todo Implement horizontal orientation support.
  *
  * In the following documentation \f$ \overline I \f$ is used to denote the set of
  * all legal intervals, \f$ I \f$ is used to identify the *ascendingly* sorted set
  * of currently existing intervals.
  *
  * The widget draws the axis that the trackers are moving along. It also draws the
  * ticks. Drawing trackers is delegated to themselves by calling
  * \ref MultiSpanSliderTracker::paint. This means that by supplying any custom
  * \ref MultiSpanSliderTracker implementation you can easily change the appearance
  * of the trackers. The supplied \ref IntSpanPainter implementation is responsible
  * for visualizing the inner intervals. By default, a null-implementation is used,
  * which simply does not visualize the inner intervals.
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 2.4.15
  */
class CARNAQT_LIB MultiSpanSlider : public QWidget
{

    Q_OBJECT

public:

    /** \brief
      * Instantiates.
      *
      * \param orientation
      *     defines whether the trackers are moving horizontally or vertically.
      *
      * \param tracker
      *     is the prototype of the trackers to use.
      *
      * \param spanPainter
      *     defines the visualization of spans.
      *
      * \param parent
      *     is passed through to the `QWidget` constructor.
      */
    explicit MultiSpanSlider
        ( Qt::Orientation orientation = Qt::Vertical
        , MultiSpanSliderTracker* tracker = new MultiSpanSliderTracker()
        , IntSpanPainter* spanPainter = new NullIntSpanPainter()
        , QWidget* parent = nullptr );


    /** \brief
      * Returns \f$ \min \{ \min J, J \in \overline I \} \f$.
      */
    int getFirst() const
    {
        return first;
    }

    /** \brief
      * Returns \f$ \max \{ \max J, J \in \overline I \} \f$.
      */
    int getLast() const
    {
        return last;
    }

    /** \brief
      * Returns the number of values spent between two ticks.
      *
      * If set to \f$0\f$, no ticks will be drawn at all. Behavior is undefined for
      * negative values.
      */
    int getTickInterval() const
    {
        return tickInterval;
    }

    /** \brief
      * Returns the number of values spent between two labeled ticks.
      *
      * If set to \f$0\f$, no labeled ticks will be drawn at all. Behavior is
      * undefined for negative values.
      */
    int getCaptionInterval() const
    {
        return captionInterval;
    }

    /** \brief
      * Returns \f$ \left| I \right| \f$.
      */
    unsigned int getSpanCount() const
    {
        return spans.size();
    }

    /** \brief
      * References a certain element within \f$ I \f$.
      */
    const base::math::Span< int >& getSpan( unsigned int index ) const
    {
        return spans[ index ];
    }

    /** \brief
      * Tells whether an invocation to \ref updateSpan would satisfy all
      * pre-conditions.
      *
      * These are in particular:
      *
      *   - `index <= 0 || getSpan( index - 1 ).last < span.first`
      *   - `index + 1 >= getSpanCount() || getSpan( index + 1 ).first > span.last`
      *   - `span.first < span.last`
      *   - `span.first >= getFirst() && span.last <= getLast()`
      */
    bool canTakeUpdatedSpan( unsigned int index, const base::math::Span< int >& span ) const;

    /** \brief
      * Manipulates a particular element within \f$ I \f$.
      *
      * \pre `index <= 0 || getSpan( index - 1 ).last < span.first`
      * \pre `index + 1 >= getSpanCount() || getSpan( index + 1 ).first > span.last`
      * \pre `span.first < span.last`
      * \pre `span.first >= getFirst() && span.last <= getLast()`
      */
    void updateSpan( unsigned int index, const base::math::Span< int >& span );

    /** \brief
      * Sets a new span visualization definition.
      */
    void setSpanPainter( IntSpanPainter* );

    /** \brief
      * References the supplied tracker prototype.
      */
    MultiSpanSliderTracker& getTracker()
    {
        return *tracker;
    }

    /** \brief
      * References the supplied tracker prototype.
      */
    const MultiSpanSliderTracker& getTracker() const
    {
        return *tracker;
    }

    /** \brief
      * References the supplied span visualization definition.
      */
    IntSpanPainter& getSpanPainter()
    {
        return *spanPainter;
    }

    /** \brief
      * References the supplied span visualization definition.
      */
    const IntSpanPainter& getSpanPainter() const
    {
        return *spanPainter;
    }

public slots:

    /** \brief
      * Sets \f$ \min \{ \min J, J \in \overline I \} \f$.
      *
      * \pre
      * No existing span becomes out of bounds by the requested boundary alternation.
      */
    void setFirst( int );

    /** \brief
      * Sets \f$ \max \{ \max J, J \in \overline I \} \f$.
      *
      * \pre
      * No existing span becomes out of bounds by the requested boundary alternation.
      */
    void setLast( int );

    /** \brief
      * Sets the number of values spent between two ticks.
      *
      * If set to \f$0\f$, no ticks will be drawn at all. Behavior is undefined for
      * negative values.
      */
    void setTickInterval( int );

    /** \brief
      * Sets the number of values spent between two labeled ticks.
      *
      * If set to \f$0\f$, no labeled ticks will be drawn at all. Behavior is
      * undefined for negative values.
      */
    void setCaptionInterval( int );

    /** \brief
      * Adds given span to \f$ I \f$.
      *
      * \pre `span.first < span.last`
      *
      * \throws std::runtime_error
      *     thrown when the new span would either overlap with another span or exceed
      *     the previously configured limits.
      */
    unsigned int addSpan( const base::math::Span< int >& span );

    /** \brief
      * Makes \f$ I = \emptyset \f$.
      */
    void removeAllSpans();

protected:

    /** \brief
      * Paints the widget.
      */
    virtual void paintEvent( QPaintEvent* );

    /** \brief
      * Performs tracker interaction.
      */
    virtual void mousePressEvent( QMouseEvent* ev );

    /** \brief
      * Performs tracker interaction.
      */
    virtual void mouseMoveEvent( QMouseEvent* ev );

    /** \brief
      * Performs tracker interaction.
      */
    virtual void mouseReleaseEvent( QMouseEvent* ev );

    /** \brief
      * Does nothing.
      *
      * Override this in order to implement some custom tracker interaction behavior.
      */
    virtual void processTrackerMousePress( base::math::Span< int >&, int index, MultiSpanSliderTracker::Role )
    {
    }

    /** \brief
      * Does nothing.
      */
    virtual void processNewSpan( base::math::Span< int >&, int index )
    {
    }

    /** \brief
      * Does nothing.
      */
    virtual void processRemovedSpan( base::math::Span< int >&, int index )
    {
    }

    /** \brief
      * Does nothing.
      */
    virtual void processModifiedSpan( base::math::Span< int >&, int index )
    {
    }

private:

    /** \brief
      * Holds whether the trackers are moving horizontally or vertically.
      */
    Qt::Orientation orientation;

    /** \brief
      * Holds a tracker prototype.
      */
    std::unique_ptr< MultiSpanSliderTracker > tracker;

    /** \brief
      * Defines the visualization of spans.
      */
    std::unique_ptr< IntSpanPainter > spanPainter;

    /** \brief
      * Holds \f$ \min \{ \min J, J \in \overline I \} \f$.
      */
    int first;

    /** \brief
      * Holds \f$ \max \{ \max J, J \in \overline I \} \f$.
      */
    int last;

    /** \brief
      * Holds the number of values spent between two ticks.
      */
    unsigned int tickInterval;

    /** \brief
      * Holds the number of values spent between two labeled ticks.
      */
    unsigned int captionInterval;

    /** \brief
      * Type definition of \f$ I \f$
      */
    typedef std::vector< base::math::Span< int > > SpanSet;

    /** \brief
      * Instantiation of \f$ I \f$
      */
    SpanSet spans;

    /** \brief
      * Maps numerical values from model space to pixels in view space and vise
      * versa.
      */
    MultiSpanSliderModelViewMapping mvm;

    /** \brief
      * Holds the index of the span, the user is currently interacting with, or
      * \f$-1\f$, when no interaction is happening.
      */
    int grabbedIndex;

    /** \brief
      * When \ref grabbedIndex is not \f$-1\f$, \ref grabbedMode determines which
      * ending of the span referenced by \c grabbedIndex is being modified.
      */
    MultiSpanSliderTracker::Role grabbedMode;

    /** \brief
     * Holds the pixel offset between the currently grabbed tracker's real position
     * and the result of mapping current mouse position to model space.
      */
    int grabOffset;

    /** \brief
      * Holds the previously captured significant mouse coordinate.
      */
    int prevMouseY;

    /** \brief
      * Holds whether a new span will be spawn next time the mouse is moved with
      * primary button pressed down while no span grabbed.
      */
    bool spawnNewSpan;

signals:

    /** \brief
      * Emitted when changes have been made to the model.
      */
    void spansChanged( Carna::qt::MultiSpanSlider& );

    /** \brief
      * Emitted when changes have been made to the model.
      */
    void spansChanged();

}; // MultiSpanSlider



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MULTISPANSLIDER_H_0874895466
