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

#include <Carna/qt/ColorPicker.h>
#include <Carna/qt/ColorPickerPainter.h>
#include <Carna/base/math.h>
#include <QColorDialog>
#include <QPainter>
#include <QWheelEvent>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorPicker
// ----------------------------------------------------------------------------------

ColorPicker::ColorPicker( const QColor& initial, bool ignoreAlpha, QWidget* parent )
    : QFrame( parent )
    , color( initial )
    , ignoreAlpha( ignoreAlpha )
{
    this->setFrameStyle( QFrame::Box | QFrame::Raised );
    this->setLineWidth( 1 );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}


QSize ColorPicker::sizeHint() const
{
    return QSize( 16, 16 );
}


void ColorPicker::setColor( const QColor& color )
{
    this->color = color;
    this->update();

    emit colorChanged();
    emit colorChanged( color );
}


void ColorPicker::mouseReleaseEvent( QMouseEvent* )
{
    QColorDialog dlg( color, this );
    if( !ignoreAlpha )
    {
        dlg.setOptions( QColorDialog::ShowAlphaChannel );
    }
    if( dlg.exec() == QDialog::Accepted )
    {
        if( !ignoreAlpha )
        {
            setColor( dlg.selectedColor() );
        }
        else
        {
            QColor color( dlg.selectedColor() );
            color.setAlpha( this->color.alpha() );
            setColor( color );
        }
    }
}


void ColorPicker::wheelEvent( QWheelEvent* ev )
{
    color.setAlpha( base::math::clamp< int >( color.alpha() + ( ev->delta() > 0 ? 2 : -2 ), 0, 255 ) );
    this->update();

    emit colorChanged();
    emit colorChanged( color );
}


void ColorPicker::paintEvent( QPaintEvent* ev )
{
    QFrame::paintEvent( ev );

    QPainter p( this );
    ColorPickerPainter cpp;
    cpp.paint( p, QRect( 2, 2, width() - 4, height() - 4 ), color, ignoreAlpha );
}



}  // namespace Carna :: qt

}  // namespace Carna
