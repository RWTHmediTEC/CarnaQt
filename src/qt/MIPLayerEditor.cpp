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

#include <Carna/qt/MIPLayerEditor.h>
#include <Carna/presets/MIPLayer.h>
#include <Carna/qt/WideColorPicker.h>
#include <Carna/qt/QColorConversion.h>
#include <QSpinBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MIPLayerEditor
// ----------------------------------------------------------------------------------

MIPLayerEditor::MIPLayerEditor( presets::MIPLayer& layer )
    : layer( layer )
    , laHuvMin( new QLabel( "From:" ) )
    , laHuvMax( new QLabel(   "To:" ) )
    , sbHuvMin( new QSpinBox() )
    , sbHuvMax( new QSpinBox() )
    , colorPicker( new WideColorPicker( toQColor( layer.color ) ) )
    , cbFunction( new QComboBox() )
{
    this->setFrameStyle( QFrame::Panel | QFrame::Raised );
    this->setLineWidth( 1 );
    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

    QVBoxLayout* master = new QVBoxLayout();
    QHBoxLayout* appearance = new QHBoxLayout();
    
    /* Setup color picker.
     */
    connect( colorPicker, SIGNAL( colorChanged( const QColor& ) ), this, SLOT( setColor( const QColor& ) ) );
    appearance->addWidget( colorPicker );

    /* Setup 'function' selector.
     */
    cbFunction->addItem( "Replace" );
    cbFunction->addItem(     "Add" );
    
    if( layer.function() == presets::MIPLayer::LAYER_FUNCTION_ADD )
    {
        cbFunction->setCurrentIndex( 1 );
    }
    else
    {
        cbFunction->setCurrentIndex( 0 );
    }

    connect( cbFunction, SIGNAL( activated( int ) ), this, SLOT( setFunction( int ) ) );
    appearance->addWidget( cbFunction );
    master->addLayout( appearance );
    
    /* Setup range selectors.
     */
    QHBoxLayout* range = new QHBoxLayout();

    sbHuvMin->setMinimum( -1024 );
    sbHuvMin->setMaximum(  3071 );
    sbHuvMin->setValue( layer.huRange.first );
    connect( sbHuvMin, SIGNAL( valueChanged( int ) ), this, SLOT( setHuvMin( int ) ) );
    
    range->addWidget( laHuvMin );
    range->addWidget( sbHuvMin );

    sbHuvMax->setMinimum( -1024 );
    sbHuvMax->setMaximum(  3071 );
    sbHuvMax->setValue( layer.huRange.last );
    connect( sbHuvMax, SIGNAL( valueChanged( int ) ), this, SLOT( setHuvMax( int ) ) );
    
    range->addWidget( laHuvMax );
    range->addWidget( sbHuvMax );

    master->addLayout( range );
    this->setLayout( master );

    master    ->setContentsMargins( 0, 0, 0, 0 );
    appearance->setContentsMargins( 5, 5, 5, 5 );
    range     ->setContentsMargins( 5, 5, 5, 5 );
}


void MIPLayerEditor::setHuvMin( int huvMin )
{
    huvMin = base::math::clamp( huvMin, sbHuvMin->minimum(), sbHuvMin->maximum() );
    huvMin = std::min( huvMin, sbHuvMax->value() );
    if( huvMin != sbHuvMin->value() )
    {
        sbHuvMin->setValue( huvMin );
        layer.huRange.first = huvMin;
        emit changed();
    }
}


void MIPLayerEditor::setHuvMax( int huvMax )
{
    huvMax = base::math::clamp( huvMax, sbHuvMax->minimum(), sbHuvMax->maximum() );
    huvMax = std::max( huvMax, sbHuvMin->value() );
    if( huvMax != sbHuvMax->value() )
    {
        sbHuvMax->setValue( huvMax );
        layer.huRange.last = huvMax;
        emit changed();
    }
}


void MIPLayerEditor::setFunction( int functionIndex )
{
    switch( functionIndex )
    {
    
    case FUNCTION_REPLACE:
        layer.setFunction( presets::MIPLayer::LAYER_FUNCTION_REPLACE );
        break;
        
    case FUNCTION_ADD:
        layer.setFunction( presets::MIPLayer::LAYER_FUNCTION_ADD );
        break;
    
    default:
        CARNA_FAIL( "Unknown MIPLayer function index." );
    
    }
    emit changed();
}


void MIPLayerEditor::setColor( const QColor& color )
{
    layer.color = toColor( color );
    emit changed();
}


QSize MIPLayerEditor::sizeHint() const
{
    const unsigned int width
        = std::max( cbFunction->sizeHint().width() + colorPicker->sizeHint().width()
            , sbHuvMin->sizeHint().width()
            + sbHuvMax->sizeHint().width()
            + laHuvMin->sizeHint().width()
            + laHuvMax->sizeHint().width() );
        
    const unsigned int height
        = sbHuvMin->sizeHint().height()
        + std::max( colorPicker->sizeHint().height(), cbFunction->sizeHint().height() );
    
    return QSize( width, height );
}



}  // namespace Carna :: qt

}  // namespace Carna
