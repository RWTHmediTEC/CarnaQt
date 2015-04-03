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
    layers->addWidget( control );
    controls.push_back( control );
    connect( control, SIGNAL( changed() ), this, SLOT( invalidate() ) );
    connect( control, SIGNAL(  ascended( MIPControlLayer& ) ), this, SLOT(  ascend( MIPControlLayer& ) ) );
    connect( control, SIGNAL( descended( MIPControlLayer& ) ), this, SLOT( descend( MIPControlLayer& ) ) );
    connect( control, SIGNAL(   removed( MIPControlLayer& ) ), this, SLOT(  remove( MIPControlLayer& ) ) );
}

    
void MIPControl::Details::ascend( MIPControlLayer& control )
{
    const auto ctrlItr = std::find( controls.begin(), controls.end(), &control );
    CARNA_ASSERT( ctrlItr != controls.end() );
    if( ctrlItr != controls.begin() )
    {
        std::iter_swap( ctrlItr, ctrlItr - 1 );
        updateLayersView();
    }
}


void MIPControl::Details::descend( MIPControlLayer& control )
{
    auto ctrlItr = std::find( controls.begin(), controls.end(), &control );
    CARNA_ASSERT( ctrlItr != controls.end() );
    if( ++ctrlItr != controls.end() )
    {
        ascend( **ctrlItr );
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
    for( auto ctrlItr = controls.begin(); ctrlItr != controls.end(); ++ctrlItr )
    {
        layers->addWidget( *ctrlItr );
    }
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

    //QHBoxLayout* const bottom_buttons = new QHBoxLayout();
    QPushButton* const buSaveColorConfig = new QPushButton( "Save" );
    QPushButton* const buLoadColorConfig = new QPushButton( "Load" );
    //bottom_buttons->addWidget( buSaveColorConfig );
    //bottom_buttons->addWidget( buLoadColorConfig );
    //bottom_buttons->addStretch( 1 );

    QVBoxLayout* master = new QVBoxLayout();
    master->addLayout( layout );
    //master->addLayout( bottom_buttons );

    //connect( buSaveColorConfig, SIGNAL( clicked() ), this, SLOT( saveColorConfig() ) );
    //connect( buLoadColorConfig, SIGNAL( clicked() ), this, SLOT( loadColorConfig() ) );

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
    mip.appendLayer( newLayer );
    pimpl->appendControl( *newLayer );
    invalidate();
}


void MIPControl::clearLayers()
{
    while( !pimpl->controls.empty() )
    {
        pimpl->controls[ 0 ]->remove();
    }
}



}  // namespace Carna :: qt

}  // namespace Carna

