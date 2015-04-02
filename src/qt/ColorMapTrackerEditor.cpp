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

#include <Carna/qt/ColorMapTrackerEditor.h>
#include <Carna/qt/ColorMapEditor.h>
#include <Carna/qt/WideColorPicker.h>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QPainter>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorMapTrackerEditor
// ----------------------------------------------------------------------------------

ColorMapTrackerEditor::ColorMapTrackerEditor( ColorMapEditor& widget
                                            , unsigned int color_index
                                            , int current_huv
                                            , QWidget* parent )
    : QWidget( parent )
    , widget( widget )
    , colorIndex( color_index )
    , validState( true )
    , sbHuv( new QSpinBox() )
{
    const unsigned int spanIndex = colorIndex / 2;
    const QColor current_color = ( colorIndex % 2 == 0 ? widget.getFirstColor( spanIndex ) : widget.getLastColor( spanIndex ) );

    QVBoxLayout* layout = new QVBoxLayout();

    WideColorPicker* colorPicker = new WideColorPicker( current_color );
    
    sbHuv->setMinimum( -1024 );
    sbHuv->setMaximum(  3071 );
    sbHuv->setValue( current_huv );

    layout->addWidget( sbHuv );
    layout->addWidget( colorPicker );

    QObject::connect( colorPicker, SIGNAL( colorChanged( const QColor& ) ), this, SLOT( updateColor( const QColor& ) ) );
    QObject::connect( sbHuv, SIGNAL( valueChanged( int ) ), this, SLOT( updateHuv( int ) ) );

    this->setLayout( layout );

    this->setContentsMargins( 10, 10, 10, 10 );
    this->setFixedSize( ColorMapEditor::trackerEditorBoxWidth
                      , ColorMapEditor::trackerEditorBoxHeight );
}


void ColorMapTrackerEditor::updateColor( const QColor& color )
{
    const unsigned int spanIndex = colorIndex / 2;

    if( colorIndex % 2 == 0 )
    {
        widget.getFirstColor( spanIndex ) = color;
    }
    else
    {
        widget.getLastColor( spanIndex ) = color;
    }

    widget.pretendDataChanged();
}


void ColorMapTrackerEditor::updateHuv( int new_huv )
{
    const unsigned int spanIndex = colorIndex / 2;
    const base::math::Span< int >& orig_span = widget.getSpan( spanIndex );
    base::math::Span< int > span = orig_span;

    if( colorIndex % 2 == 0 )
    {
        span.first = new_huv;
    }
    else
    {
        span.last = new_huv;
    }

    if( orig_span == span )
    {
        return; // avoids feedback effects
    }

    if( widget.canTakeUpdatedSpan( spanIndex, span ) )
    {
        try
        {
            widget.updateSpan( spanIndex, span );
        }
        catch( ... )
        {
            validState = false;
            throw;
        }
        validState = true;
        this->update();
    }
    else
    {
        validState = false;
    }
}


void ColorMapTrackerEditor::paintEvent( QPaintEvent* ev )
{
    if( !validState )
    {
        QPainter painter( this );

        painter.setPen( Qt::NoPen );
        painter.setBrush( QBrush( qRgb( 255, 150, 150 ) ) );

        painter.drawRect( this->contentsRect() );
    }

    QWidget::paintEvent( ev );
}


void ColorMapTrackerEditor::setCurrentHuv( int huv )
{
    sbHuv->setValue( huv );
    validState = true;

    this->update();
}



}  // namespace Carna :: qt

}  // namespace Carna
