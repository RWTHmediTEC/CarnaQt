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

#include <Carna/qt/MIPControl.h>
#include <Carna/qt/MIPControlDetails.h>
#include <Carna/qt/MIPControlLayer.h>
#include <Carna/presets/MIPLayer.h>
#include <Carna/presets/MIPStage.h>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDomDocument>
#include <QDomElement>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MIPControl :: Details
// ----------------------------------------------------------------------------------

MIPControl::Details::Details( MIPControl& self )
    : self( self )
    , layers( new QVBoxLayout() )
{
}


void MIPControl::Details::invalidate()
{
    self.invalidate();
}


void MIPControl::Details::appendControl( presets::MIPLayer& layer )
{
    MIPControlLayer* const control = new MIPControlLayer( self.mip, layer );
    controls.push_back( control );
    connect( control, SIGNAL( changed() ), this, SLOT( invalidate() ) );
    connect( control, SIGNAL(  ascended( MIPControlLayer& ) ), this, SLOT(  ascend( MIPControlLayer& ) ) );
    connect( control, SIGNAL( descended( MIPControlLayer& ) ), this, SLOT( descend( MIPControlLayer& ) ) );
    connect( control, SIGNAL(   removed( MIPControlLayer& ) ), this, SLOT(  remove( MIPControlLayer& ) ) );
    
    /* Insert the widget after the stretching space.
     */
    layers->insertWidget( 1, control );
}

    
void MIPControl::Details::ascend( MIPControlLayer& control )
{
    const auto ctrlItr = std::find( controls.begin(), controls.end(), &control );
    CARNA_ASSERT( ctrlItr != controls.end() );
    const auto nextCtrlItr = ctrlItr + 1;
    if( nextCtrlItr != controls.end() )
    {
        std::iter_swap( ctrlItr, nextCtrlItr );
        updateLayersView();
    }
}


void MIPControl::Details::descend( MIPControlLayer& control )
{
    const auto ctrlItr = std::find( controls.begin(), controls.end(), &control );
    CARNA_ASSERT( ctrlItr != controls.end() );
    if( ctrlItr != controls.begin() )
    {
        std::iter_swap( ctrlItr, ctrlItr - 1 );
        updateLayersView();
    }
}


void MIPControl::Details::remove( MIPControlLayer& control )
{
    const auto ctrlItr = std::find( controls.begin(), controls.end(), &control );
    CARNA_ASSERT( ctrlItr != controls.end() );
    controls.erase( ctrlItr );
}


void MIPControl::Details::updateLayersView()
{
    /* Remove all control-widgets from layout.
     */
    for( auto ctrlItr = controls.begin(); ctrlItr != controls.end(); ++ctrlItr )
    {
        layers->removeWidget( *ctrlItr );
    }
    
    /* Re-add all control-widgets in proper order.
     */
    for( auto ctrlItr = controls.rbegin(); ctrlItr != controls.rend(); ++ctrlItr )
    {
        layers->addWidget( *ctrlItr );
    }
}


void MIPControl::Details::appendLayerWithoutInvalidating( presets::MIPLayer* layer )
{
    self.mip.appendLayer( layer );
    appendControl( *layer );
}



// ----------------------------------------------------------------------------------
// MIPControl
// ----------------------------------------------------------------------------------

MIPControl::MIPControl( presets::MIPStage& mip, QWidget* parent )
    : VolumeRenderingControl( mip, parent )
    , pimpl( new Details( *this ) )
    , mip( mip )
{
    QVBoxLayout* const layout = new QVBoxLayout();
    
    /* Add 'sample rate' configurator.
     */
    QFormLayout* const general = new QFormLayout();
    general->addRow( "Sample Rate:", sbSampleRate );
    layout->addLayout( general );
    
    /* Add layers list.
     */
    QPushButton* const buAddChannel = new QPushButton( "&Add Layer" );
    connect( buAddChannel, SIGNAL( clicked() ), this, SLOT( appendLayer() ) );
    layout->addWidget( buAddChannel );

    pimpl->layers->addStretch();
    layout->addLayout( pimpl->layers );
    
    layout->setContentsMargins( 0, 0, 0, 0 );
    pimpl->layers->setContentsMargins( 0, 0, 0, 0 );

    QHBoxLayout* const bottom_buttons = new QHBoxLayout();
    QPushButton* const buSaveColorConfig = new QPushButton( "Save" );
    QPushButton* const buLoadColorConfig = new QPushButton( "Load" );
    bottom_buttons->addWidget( buSaveColorConfig );
    bottom_buttons->addWidget( buLoadColorConfig );
    bottom_buttons->addStretch( 1 );

    QVBoxLayout* master = new QVBoxLayout();
    master->addLayout( layout );
    master->addLayout( bottom_buttons );

    connect( buSaveColorConfig, SIGNAL( clicked() ), this, SLOT( saveColorConfig() ) );
    connect( buLoadColorConfig, SIGNAL( clicked() ), this, SLOT( loadColorConfig() ) );

    this->setLayout( master );

    /* Initialize layers list.
     */
    for( std::size_t layerIdx = 0; layerIdx < mip.layersCount(); ++layerIdx )
    {
        presets::MIPLayer& layer = mip.layer( layerIdx );
        pimpl->appendControl( layer );
    }
}


MIPControl::~MIPControl()
{
}


void MIPControl::appendLayer()
{
    const base::Color color = pimpl->nextColor++;
    presets::MIPLayer* const newLayer = new presets::MIPLayer( -1024, 3071, color );
    appendLayer( newLayer );
}


void MIPControl::appendLayer( presets::MIPLayer* layer )
{
    pimpl->appendLayerWithoutInvalidating( layer );
    invalidate();
}


void MIPControl::clearLayers()
{
    while( !pimpl->controls.empty() )
    {
        pimpl->controls[ 0 ]->remove();
    }
}


void MIPControl::saveColorConfig()
{
    const QString filename = QFileDialog::getSaveFileName
        ( this
        , "Save MIP Layers"
        , ""
        , "XML-Files (*.XML)"
        , 0
        , QFileDialog::DontResolveSymlinks
        | QFileDialog::HideNameFilterDetails );

    if( filename.isEmpty() )
    {
        return;
    }

    QDomDocument dom;
    dom.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" );

    QDomElement root = dom.createElement( "MIPLayers" );
    dom.appendChild( root );

 // see: http://www.digitalfanatics.org/projects/qt_tutorial/chapter09.html

    for( std::size_t layerIdx = 0; layerIdx < mip.layersCount(); ++layerIdx )
    {
        const presets::MIPLayer& layer = mip.layer( layerIdx );
        QDomElement node = dom.createElement( "Layer" );

        node.setAttribute( "minHuv", layer.huRange.first );
        node.setAttribute( "maxHuv", layer.huRange.last );

        const base::Color& color = layer.color;
        node.setAttribute( "colorR", color.r );
        node.setAttribute( "colorG", color.g);
        node.setAttribute( "colorB", color.b );
        node.setAttribute( "colorA", color.a );

        const unsigned int functionIdx = MIPLayerEditor::functionIndex( layer.function() );
        const std::string functionName = MIPLayerEditor::functionName( functionIdx );
        node.setAttribute( "function", QString::fromStdString( functionName ) );

        root.appendChild( node );
    }

    QFile file( filename );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
        throw base::CarnaException
            ( "Save MIP Layers"
            , "Failed opening file for writing."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }

    QTextStream stream( &file );
    stream << dom.toString();

    file.close();
}


void MIPControl::loadColorConfig()
{
    const QString filename = QFileDialog::getOpenFileName
        ( this
        , "Load MIP Layers"
        , ""
        , "XML-Files (*.xml)"
        , 0
        , QFileDialog::ReadOnly
        | QFileDialog::HideNameFilterDetails );

    if( filename.isEmpty() )
    {
        return;
    }

    QFile file( filename );
    if( !file.open( QIODevice::ReadOnly ) )
    {
        throw base::CarnaException
            ( "Load MIP Layers"
            , "Failed opening file for reading."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }

    this->clearLayers();

    struct unexpected_format_exception { };
    try
    {

        QDomDocument dom;
        if( !dom.setContent( &file ) )
        {
            throw unexpected_format_exception();
        }

        QDomElement root = dom.documentElement();
        if( root.tagName() != "MIPLayers" )
        {
            throw unexpected_format_exception();
        }

        QDomNode n = root.firstChild();
        while( !n.isNull() )
        {
            QDomElement e = n.toElement();
            if( !e.isNull() )
            {
                if( e.tagName() != "Layer" )
                {
                    throw unexpected_format_exception();
                }

                const int huv0 = e.attribute( "minHuv" ).toInt();
                const int huv1 = e.attribute( "maxHuv" ).toInt();

                const int r = e.attribute( "colorR" ).toInt();
                const int g = e.attribute( "colorG" ).toInt();
                const int b = e.attribute( "colorB" ).toInt();
                const int a = e.attribute( "colorA" ).toInt();
                const base::Color color( r, g, b, a );

                const std::string functionName = e.attribute( "function" ).toStdString();
                const unsigned int functionIdx = MIPLayerEditor::functionIndex( functionName );
                const base::BlendFunction& function = MIPLayerEditor::function( functionIdx );
                
                presets::MIPLayer* const newLayer = new presets::MIPLayer( huv0, huv1, color, function );
                pimpl->appendLayerWithoutInvalidating( newLayer );
            }

            n = n.nextSibling();
        }
        
        invalidate();
    }
    catch( const unexpected_format_exception& )
    {
        throw base::CarnaException
            ( "Load MIP Layers"
            , "Unexpected file format."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }
}



}  // namespace Carna :: qt

}  // namespace Carna

