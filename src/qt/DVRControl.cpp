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

#include <Carna/qt/DVRControl.h>
#include <Carna/qt/ExpandableGroupBox.h>
#include <Carna/qt/QColorConversion.h>
#include <Carna/presets/DVRStage.h>
#include <Carna/base/CarnaException.h>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QTimer>
#include <QFormLayout>
#include <QPushButton>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DVRControl
// ----------------------------------------------------------------------------------

DVRControl::DVRControl( presets::DVRStage& dvr, QWidget* parent )
    : VolumeRenderingControl( dvr, parent )
    , dvr( dvr )
    , sbTranslucence( new QDoubleSpinBox )
    , slDiffuseLight( new QSlider( Qt::Horizontal ) )
    , colorMapEditor( new ColorMapEditor() )
{
    colorMapEditor->setFirst( -1024 );
    colorMapEditor->setLast( 3071 );
    colorMapEditor->setTickInterval( 200 );
    colorMapEditor->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::MinimumExpanding );

    /* Compose 'General' section.
     */
    sbTranslucence->setMinimum( 0 );
    sbTranslucence->setMaximum( std::numeric_limits< float >::max() );
    sbTranslucence->setSingleStep( 1 );
    sbTranslucence->setDecimals( 2 );
    sbTranslucence->setValue( dvr.translucence() );
    
    connect( sbTranslucence, SIGNAL( valueChanged( double ) ), this, SLOT( setTranslucence( double ) ) );

    ExpandableGroupBox* const gbGeneral = new ExpandableGroupBox( "General", true );
    QFormLayout* const general = new QFormLayout();
    gbGeneral->child()->setLayout( general );
    gbGeneral->child()->layout()->setContentsMargins( 0, 0, 0 ,0 );

    general->addRow( "Sample Rate:" , sbSampleRate );
    general->addRow( "Translucence:", sbTranslucence );
    
    /* Compose 'Lighting' section.
     */
    slDiffuseLight->setMinimum( 0 );
    slDiffuseLight->setMaximum( 100 );
    slDiffuseLight->setTickInterval( 10 );
    
    connect( slDiffuseLight, SIGNAL( valueChanged( int ) ), this, SLOT( setDiffuseLight( int ) ) );

    ExpandableGroupBox* const gbLight = new ExpandableGroupBox( "Lighting", false );
    QVBoxLayout* const lightMaster = new QVBoxLayout();
    gbLight->child()->setLayout( lightMaster );
    gbLight->child()->layout()->setContentsMargins( 0, 0, 0 ,0 );
    
    QLabel* const lightCaptionAmbient = new QLabel( "Ambient" );
    QLabel* const lightCaptionDiffuse = new QLabel( "Diffuse" );
    lightCaptionAmbient->setAlignment( Qt::AlignLeft  | Qt::AlignTop );
    lightCaptionDiffuse->setAlignment( Qt::AlignRight | Qt::AlignTop );
    
    QHBoxLayout* const lightCaptions = new QHBoxLayout();
    lightCaptions->addWidget( lightCaptionAmbient );
    lightCaptions->addWidget( lightCaptionDiffuse );

    lightMaster->addWidget( slDiffuseLight );
    lightMaster->addLayout( lightCaptions );

    /* Compose color map saving/loading buttons.
     */
    QHBoxLayout* const bottom_buttons = new QHBoxLayout();
    QPushButton* const buSaveColorConfig = new QPushButton( "Save" );
    QPushButton* const buLoadColorConfig = new QPushButton( "Load" );
    bottom_buttons->addWidget( buSaveColorConfig );
    bottom_buttons->addWidget( buLoadColorConfig );
    bottom_buttons->addStretch( 1 );

    /* Put everything together.
     */
    QVBoxLayout* master = new QVBoxLayout();
    master->addWidget( gbGeneral );
    master->addWidget( gbLight );
    master->addWidget( colorMapEditor );
    master->addLayout( bottom_buttons );

    this->setLayout( master );

    connect( colorMapEditor, SIGNAL( spansChanged() ), this, SLOT( updateColorMap() ) ); 
    connect( buSaveColorConfig, SIGNAL( clicked() ), this, SLOT( saveColorConfig() ) );
    connect( buLoadColorConfig, SIGNAL( clicked() ), this, SLOT( loadColorConfig() ) );
}


DVRControl::~DVRControl()
{
}


void DVRControl::updateColorMap()
{
    dvr.clearColorMap();
    for( unsigned int spanIndex = 0; spanIndex < colorMapEditor->getSpanCount(); ++spanIndex )
    {
        const base::math::Span< int >& span = colorMapEditor->getSpan( spanIndex );
        
        const base::math::Span< base::HUV > huRange
            ( static_cast< base::HUV >( span.first )
            , static_cast< base::HUV >( span.last ) );

        const base::math::Span< base::Color > colorRange
            ( toColor( colorMapEditor->getFirstColor( spanIndex ) )
            , toColor( colorMapEditor->getLastColor ( spanIndex ) ) );

        dvr.writeColorMap( huRange, colorRange );
    }
    RenderStageControl::invalidate();
}


void DVRControl::saveColorConfig()
{
    const QString filename = QFileDialog::getSaveFileName
        ( this
        , "Save Color Map"
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

    QDomElement root = dom.createElement( "AbsorptionColorTable" );
    dom.appendChild( root );

    root.setAttribute( "diffuseLight", dvr.diffuseLight() );

 // see: http://www.digitalfanatics.org/projects/qt_tutorial/chapter09.html

    for( unsigned int i = 0; i < colorMapEditor->getSpanCount(); ++i )
    {
        const base::math::Span< int >& span = colorMapEditor->getSpan( i );
        const QColor& color0 = colorMapEditor->getFirstColor( i );
        const QColor& color1 = colorMapEditor->getLastColor( i );

        QDomElement node = dom.createElement( "Channel" );

        node.setAttribute( "minHuv", span.first );
        node.setAttribute( "maxHuv", span.last  );

        node.setAttribute( "minColorR", color0.red  () );
        node.setAttribute( "minColorG", color0.green() );
        node.setAttribute( "minColorB", color0.blue () );
        node.setAttribute( "minColorA", color0.alpha() );

        node.setAttribute( "maxColorR", color1.red  () );
        node.setAttribute( "maxColorG", color1.green() );
        node.setAttribute( "maxColorB", color1.blue () );
        node.setAttribute( "maxColorA", color1.alpha() );

        root.appendChild( node );
    }

    QFile file( filename );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
        throw base::CarnaException
            ( "Save Color Map"
            , "Failed opening file for writing."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }

    QTextStream stream( &file );
    stream << dom.toString();

    file.close();
}


void DVRControl::loadColorConfig()
{
    const QString filename = QFileDialog::getOpenFileName
        ( this
        , "Load Color Map"
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
            ( "Load Color Map"
            , "Failed opening file for reading."
            , ( "Filename: '" + filename + "'" ).toStdString() );
    }
    
    colorMapEditor->removeAllSpans();

    struct unexpected_format_exception { };
    try
    {

        QDomDocument dom;
        if( !dom.setContent( &file ) )
        {
            throw unexpected_format_exception();
        }

        QDomElement root = dom.documentElement();
        if( root.tagName() != "AbsorptionColorTable" )
        {
            throw unexpected_format_exception();
        }

     // read lighting settings

        float diffuseLight = slDiffuseLight->value() / static_cast< float >( slDiffuseLight->maximum() );
        diffuseLight = root.attribute( "diffuseLight", QString::number( diffuseLight ) ).toFloat();
        slDiffuseLight->setValue( base::math::round_ui( diffuseLight * slDiffuseLight->maximum() ) );

     // read color map

        QDomNode n = root.firstChild();
        while( !n.isNull() )
        {
            QDomElement e = n.toElement();
            if( !e.isNull() )
            {
                if( e.tagName() != "Channel" )
                {
                    throw unexpected_format_exception();
                }

                const int huv0 = e.attribute( "minHuv" ).toInt();
                const int huv1 = e.attribute( "maxHuv" ).toInt();

                const int r0 = e.attribute( "minColorR" ).toInt();
                const int g0 = e.attribute( "minColorG" ).toInt();
                const int b0 = e.attribute( "minColorB" ).toInt();
                const int a0 = e.attribute( "minColorA" ).toInt();

                const int r1 = e.attribute( "maxColorR" ).toInt();
                const int g1 = e.attribute( "maxColorG" ).toInt();
                const int b1 = e.attribute( "maxColorB" ).toInt();
                const int a1 = e.attribute( "maxColorA" ).toInt();
    
                const int span = colorMapEditor->addSpan( base::math::Span< int >( huv0,  huv1 ) );

                colorMapEditor->getFirstColor( span ) = QColor( r0, g0, b0, a0 );
                colorMapEditor->getLastColor ( span ) = QColor( r1, g1, b1, a1 );
            }

            n = n.nextSibling();
        }
    }
    catch( const unexpected_format_exception& )
    {
        throw base::CarnaException
            ( "Load Color Map"
            , "Unexpected file format." );
    }

    this->updateColorMap();
}


void DVRControl::setTranslucence( double translucence )
{
    const float translucenceF = static_cast< float >( translucence );
    if( !base::math::isEqual( translucenceF, dvr.translucence() ) )
    {
        dvr.setTranslucence( translucenceF );
        RenderStageControl::invalidate();
    }
}


void DVRControl::setDiffuseLight( double diffuseLight )
{
    const float diffuseLightF = static_cast< float >( diffuseLight );
    if( !base::math::isEqual( diffuseLightF, dvr.diffuseLight() ) )
    {
        dvr.setDiffuseLight( diffuseLightF );
        RenderStageControl::invalidate();
    }
}


void DVRControl::setDiffuseLight( int diffuseLight )
{
    if( slDiffuseLight->isEnabled() )
    {
        setDiffuseLight( diffuseLight / static_cast< double >( slDiffuseLight->maximum() ) );
    }
}


void DVRControl::updateLightingState()
{
    slDiffuseLight->setEnabled( dvr.isLightingUsed() );
    if( dvr.isLightingUsed() )
    {
        slDiffuseLight->setValue( base::math::round_ui( dvr.diffuseLight() * slDiffuseLight->maximum() ) );
    }
    else
    {
        slDiffuseLight->setValue( 0 );
    }
}


void DVRControl::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    QTimer::singleShot( 0, this, SLOT( updateLightingState() ) );
}


void DVRControl::onRenderingFinished()
{
    updateLightingState();
}



}  // namespace Carna :: qt

}  // namespace Carna
