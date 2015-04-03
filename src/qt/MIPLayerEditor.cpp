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
    cbFunction->setCurrentIndex( functionIndex( layer.function() ) );

    connect( cbFunction, SIGNAL( activated( int ) ), this, SLOT( setFunction( int ) ) );
    appearance->addWidget( cbFunction );
    master->addLayout( appearance );
    
    /* Setup range selectors.
     */
    QHBoxLayout* range = new QHBoxLayout();

    sbHuvMin->setMinimum( -1024 );
    sbHuvMin->setMaximum(  3071 );
    sbHuvMin->setValue( layer.huRange.first );
    sbHuvMin->setSingleStep( 100 );
    connect( sbHuvMin, SIGNAL( valueChanged( int ) ), this, SLOT( setHuvMin( int ) ) );
    
    range->addWidget( laHuvMin );
    range->addWidget( sbHuvMin );

    sbHuvMax->setMinimum( -1024 );
    sbHuvMax->setMaximum(  3071 );
    sbHuvMax->setSingleStep( 100 );
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
    if( huvMin != layer.huRange.first )
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
    if( huvMax != layer.huRange.last )
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


std::string MIPLayerEditor::functionName( unsigned int functionIndex )
{
    switch( functionIndex )
    {
    
    case FUNCTION_REPLACE:
        return "Replace";
        
    case FUNCTION_ADD:
        return "Add";
    
    default:
        CARNA_FAIL( "Unknown MIPLayer function index." );
    
    }
}


unsigned int MIPLayerEditor::functionIndex( const std::string& functionName )
{
    if( functionName == "Replace" )
    {
        return FUNCTION_REPLACE;
    }
    else
    if( functionName == "Add" )
    {
        return FUNCTION_ADD;
    }
    else
    {
        CARNA_FAIL( "Unknown MIPLayer function name." );
    }
}


unsigned int MIPLayerEditor::functionIndex( const base::BlendFunction& function )
{
    if( function == presets::MIPLayer::LAYER_FUNCTION_REPLACE )
    {
        return FUNCTION_REPLACE;
    }
    else
    if( function == presets::MIPLayer::LAYER_FUNCTION_ADD )
    {
        return FUNCTION_ADD;
    }
    else
    {
        CARNA_FAIL( "Unknown MIPLayer function." );
    }
}


const base::BlendFunction& MIPLayerEditor::function( unsigned int functionIndex )
{
    switch( functionIndex )
    {
    
    case FUNCTION_REPLACE:
        return presets::MIPLayer::LAYER_FUNCTION_REPLACE;
        
    case FUNCTION_ADD:
        return presets::MIPLayer::LAYER_FUNCTION_ADD;
    
    default:
        CARNA_FAIL( "Unknown MIPLayer function index." );
    
    }
}



}  // namespace Carna :: qt

}  // namespace Carna
