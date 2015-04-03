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

#include <Carna/qt/ColorMapEditor.h>
#include <Carna/qt/ColorMapTracker.h>
#include <Carna/qt/ColorMapSpanPainter.h>
#include <Carna/qt/ColorMapTrackerEditor.h>
#include <QPainter>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// toQColor
// ----------------------------------------------------------------------------------

inline QColor toQColor( const base::Color& color )
{
    return QColor( color.r, color.g, color.b, color.a );
}



// ----------------------------------------------------------------------------------
// ColorMapEditor
// ----------------------------------------------------------------------------------

const int ColorMapEditor::trackerEditorBoxWidth  = 120;
const int ColorMapEditor::trackerEditorBoxHeight = 100;


ColorMapEditor::ColorMapEditor( QWidget* parent )
    : MultiSpanSlider( Qt::Vertical
                     , new ColorMapTracker()
                     , new ColorMapSpanPainter()
                     , parent )
    , selectedSpan( 0 )
    , beginConfigWidget( 0 )
    , endConfigWidget( 0 )
{
    static_cast< ColorMapTracker& >( getTracker() ).setWidget( *this );
    static_cast< ColorMapSpanPainter& >( getSpanPainter() ).setWidget( *this );

    this->setMinimumHeight( trackerEditorBoxHeight * 2 );
}


ColorMapEditor::~ColorMapEditor()
{
    if( beginConfigWidget )
    {
        delete beginConfigWidget;
        delete endConfigWidget;
    }
}


void ColorMapEditor::processTrackerMousePress( base::math::Span< int >& span, int index, MultiSpanSliderTracker::Role mode )
{
    if( selectedSpan != &span )
    {
        selectedSpan = &span;

        if( beginConfigWidget )
        {
            beginConfigWidget->hide();
            endConfigWidget->hide();

            delete beginConfigWidget;
            delete endConfigWidget;

            beginConfigWidget = 0;
            endConfigWidget = 0;
        }

        beginConfigWidget = new ColorMapTrackerEditor( *this, 2 * index, span.first, this );
        endConfigWidget = new ColorMapTrackerEditor( *this, 2 * index + 1, span.last, this );

        layoutSpanConfigWidgets();

        beginConfigWidget->show();
        endConfigWidget->show();

        this->update();
    }
}


void ColorMapEditor::processNewSpan( base::math::Span< int >&, int index )
{
    QColor color0( toQColor( nextColor ) );
    QColor color1( toQColor( nextColor ) );

    ++nextColor;

    color0.setAlphaF( 0.f );
    color1.setAlphaF( 1.f );

    colors.insert( colors.begin() + 2 * index    , color0 );
    colors.insert( colors.begin() + 2 * index + 1, color1 );
}


void ColorMapEditor::processRemovedSpan( base::math::Span< int >& span, int index )
{
    colors.erase( colors.begin() + 2 * index, colors.begin() + 2 * index + 2 );

    if( selectedSpan == &span )
    {
        if( beginConfigWidget )
        {
            beginConfigWidget->hide();
            endConfigWidget->hide();

            delete beginConfigWidget;
            delete endConfigWidget;

            beginConfigWidget = 0;
            endConfigWidget = 0;
        }

        selectedSpan = 0;
        this->update();
    }
}


void ColorMapEditor::processModifiedSpan( base::math::Span< int >& span, int index )
{
    if( selectedSpan == &span )
    {
        if( beginConfigWidget )
        {
            beginConfigWidget->setCurrentHuv( span.first );
            endConfigWidget  ->setCurrentHuv( span.last  );
        }
    }
}


void ColorMapEditor::paintEvent( QPaintEvent* ev )
{
    MultiSpanSlider::paintEvent( ev );

    if( selectedSpan )
    {
        QPainter painter( this );

        const static int radius = 5;

        const QRect rect( width() - trackerEditorBoxWidth - 1, 0, trackerEditorBoxWidth, trackerEditorBoxHeight );

        painter.setPen( Qt::NoPen );
        painter.setBrush( Qt::white );

        painter.drawRoundedRect( rect, radius, radius );
        painter.drawRoundedRect( rect.translated( 0, height() - trackerEditorBoxHeight ), radius, radius );
    }
}


void ColorMapEditor::resizeEvent( QResizeEvent* )
{
    if( beginConfigWidget )
    {
        layoutSpanConfigWidgets();
    }
}


void ColorMapEditor::layoutSpanConfigWidgets()
{
    beginConfigWidget->move( this->width() - trackerEditorBoxWidth, 0 );
    endConfigWidget->move( this->width() - trackerEditorBoxWidth, this->height() - trackerEditorBoxHeight );
}



}  // namespace Carna :: qt

}  // namespace Carna
