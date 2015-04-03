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

#ifndef COLORMAPEDITOR_H_0874895466
#define COLORMAPEDITOR_H_0874895466

/** \file   ColorMapEditor.h
  * \brief  Defines \ref Carna::qt::ColorMapEditor.
  */

#include <Carna/qt/MultiSpanSlider.h>
#include <Carna/base/RotatingColor.h>
#include <deque>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorMapEditor
// ----------------------------------------------------------------------------------

/** \brief
  * Implements user interface for `presets::DVRStage` color map configuration.
  *
  * \author Leonid Kostrykin
  * \date   16.8.11 - 2.4.15
  */
class CARNAQT_LIB ColorMapEditor : public MultiSpanSlider
{

    Q_OBJECT

public:

    /** \brief
      * Holds width of box drawn around ColorMapTrackerEditor instances. 
      */
    static const int trackerEditorBoxWidth;

    /** \brief
      * Holds height of box drawn around ColorMapTrackerEditor instances. 
      */
    static const int trackerEditorBoxHeight;

    /** \brief
      * Instantiates.
      */
    explicit ColorMapEditor( QWidget* parent = 0 );

    /** \brief
      * Does nothing.
      */
    virtual ~ColorMapEditor();

    /** \brief
      * References the currently selected span or `nullptr` if no span is selected.
      */
    base::math::Span< int >* getSelectedSpan()
    {
        return selectedSpan;
    }

    /** \brief
      * References the currently selected span or `nullptr` if no span is selected.
      */
    const base::math::Span< int >* getSelectedSpan() const
    {
        return selectedSpan;
    }

    /** \brief
      * References the color assigned to the beginning tracker of given span.
      */
    QColor& getFirstColor( int span_index )
    {
        return colors[ 2 * span_index ];
    }

    /** \brief
      * References the color assigned to the ending tracker of given span.
      */
    QColor& getLastColor( int span_index )
    {
        return colors[ 2 * span_index + 1 ];
    }

    /** \brief
      * References the color assigned to the beginning tracker of given span.
      */
    const QColor& getFirstColor( int span_index ) const
    {
        return const_cast< ColorMapEditor* >( this )->getFirstColor( span_index );
    }

    /** \brief
      * References the color assigned to the ending tracker of given span.
      */
    const QColor& getLastColor( int span_index ) const
    {
        return const_cast< ColorMapEditor* >( this )->getLastColor( span_index );
    }

    /** \brief Emits signals indicating this widget's state has changed.
      *
      * Also posts a repaint event.
      */
    void pretendDataChanged()
    {
        emit spansChanged( *this );
        emit spansChanged();
        this->update();
    }

protected:

    virtual void paintEvent( QPaintEvent* ) override;

    /** \brief
      * Updates the layouting of the \ref ColorMapTrackerEditor instances.
      *
      * This is done by the invocation of
      * \ref ColorMapEditor::layoutSpanConfigWidgets.
      */
    virtual void resizeEvent( QResizeEvent* ) override;

    /** \brief
      * Updates span selection.
      */
    virtual void processTrackerMousePress( base::math::Span< int >&, int index, MultiSpanSliderTracker::Role ) override;

    /** \brief
      * Updates color map.
      */
    virtual void processNewSpan( base::math::Span< int >&, int index ) override;

    /** \brief
      * Updates color map and span selection.
      */
    virtual void processRemovedSpan( base::math::Span< int >&, int index ) override;

    /** \brief
      * Updates ColorMapTrackerEditor instances for currently selected span.
      */
    virtual void processModifiedSpan( base::math::Span< int >&, int index ) override;

private: // TODO: move to PIMPL

    /** \brief
      * References the currently selected span.
      */
    base::math::Span< int >* selectedSpan;

    /** \brief
      * Holds the color map.
      */
    std::deque< QColor > colors;

    /** \brief
      * Holds the color which will be assigned to the span created next.
      */
    base::RotatingColor nextColor;

    /** \brief
      * References the editor for the currently selected beginning tracker or is
      * \f$0\f$ when no span is selected.
      */
    ColorMapTrackerEditor* beginConfigWidget;

    /** \brief
      * References the editor for the currently selected ending tracker or is
      * \f$0\f$ when no span is selected.
      */
    ColorMapTrackerEditor* endConfigWidget;

private slots:

    /** \brief
      * Updates the layouting of the \ref ColorMapTrackerEditor instances.
      */
    void layoutSpanConfigWidgets();

}; // ColorMapEditor



}  // namespace Carna :: qt

}  // namespace Carna

#endif // COLORMAPEDITOR_H_0874895466
