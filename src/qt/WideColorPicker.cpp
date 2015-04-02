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

#include <Carna/qt/WideColorPicker.h>
#include <Carna/qt/ColorPicker.h>
#include <QSpinBox>
#include <QHBoxLayout>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// WideColorPicker
// ----------------------------------------------------------------------------------

WideColorPicker::WideColorPicker( const QColor& initial, QWidget* parent )
    : QWidget( parent )
    , colorPicker( new ColorPicker( initial, true ) )
    , sbAlpha( new QSpinBox() )
{
    QHBoxLayout* layout = new QHBoxLayout();

    sbAlpha->setMinimum(   0 );
    sbAlpha->setMaximum( 100 );
    sbAlpha->setValue( static_cast< int >( initial.alphaF() * 100 + 0.5f ) );

    layout->addWidget( colorPicker );
    layout->addWidget( sbAlpha );

    this->setLayout( layout );
    this->layout()->setContentsMargins( 0, 0, 0, 0 );

    connect( sbAlpha, SIGNAL( valueChanged( int ) ), this, SLOT( updateAlpha( int ) ) );
    connect( colorPicker, SIGNAL( colorChanged( const QColor& ) ), this, SLOT( updateColor( const QColor& ) ) );
}


const QColor& WideColorPicker::getColor() const
{
    return colorPicker->getColor();
}


void WideColorPicker::setColor( const QColor& color )
{
    colorPicker->setColor( color );
    sbAlpha->setValue( static_cast< int >( color.alphaF() * 100 + 0.5f ) );
}


void WideColorPicker::updateColor( const QColor& color )
{
    emit colorChanged();
    emit colorChanged( color );
}


void WideColorPicker::updateAlpha( int alpha )
{
    QColor color = colorPicker->getColor();
    color.setAlphaF( alpha / 100.f );
    colorPicker->setColor( color );
}



}  // namespace Carna :: qt

}  // namespace Carna
