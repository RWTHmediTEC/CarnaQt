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

#include <Carna/qt/DRRControl.h>
#include <Carna/qt/ExpandableGroupBox.h>
#include <Carna/base/math.h>
#include <Carna/presets/DRRStage.h>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DRRControl
// ----------------------------------------------------------------------------------

DRRControl::DRRControl( presets::DRRStage& drr, QWidget* parent )
    : QWidget( parent )
    , RenderStageControl( drr )
    , drr( drr )
    , sbWaterAttenuation( new QDoubleSpinBox() )
    , sbBrightness      ( new QDoubleSpinBox() )
    , sbLowerThreshold  ( new QSpinBox() )
    , sbUpperThreshold  ( new QSpinBox() )
    , sbUpperMultiplier ( new QDoubleSpinBox() )
{
    QVBoxLayout* const layout = new QVBoxLayout();

 // DRR parameters

    ExpandableGroupBox* const gbDRR = new ExpandableGroupBox( "DRR Parameters", true );
    QFormLayout* const drrParams = new QFormLayout();
    gbDRR->child()->setLayout( drrParams );
    gbDRR->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( gbDRR );

 // water attenuation

    drrParams->addRow( "Water Attenuation", sbWaterAttenuation );

    sbWaterAttenuation->setRange( 0, std::numeric_limits< float >::max() );
    sbWaterAttenuation->setValue( drr.waterAttenuation() );
    sbWaterAttenuation->setSingleStep( 0.1 );

    connect( sbWaterAttenuation, SIGNAL( valueChanged( double ) ), this, SLOT( setWaterAttenuation( double ) ) );

 // base intensity

    drrParams->addRow( "Intensity", sbBrightness );

    sbBrightness->setRange( 0, std::numeric_limits< float >::max() );
    sbBrightness->setValue( drr.baseIntensity() );
    sbBrightness->setDecimals( 2 );
    sbBrightness->setSingleStep( 0.1 );

    connect( sbBrightness, SIGNAL( valueChanged( double ) ), this, SLOT( setBrightness( double ) ) );

 // threshold filtering / bone enhancement

    ExpandableGroupBox* const gbFiltering = new ExpandableGroupBox( "Threshold Filtering / Bone Enhancement", true );
    QFormLayout* const filtering = new QFormLayout();
    gbFiltering->child()->setLayout( filtering );
    gbFiltering->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( gbFiltering );

 // lower threshold

    filtering->addRow( "Lower Threshold", sbLowerThreshold );

    sbLowerThreshold->setRange( -1024, 3071 );
    sbLowerThreshold->setValue( drr.lowerThreshold() );
    sbLowerThreshold->setSingleStep( 10 );

    connect( sbLowerThreshold, SIGNAL( valueChanged( int ) ), this, SLOT( setLowerThreshold( int ) ) );

 // upper threshold

    filtering->addRow( "Upper Threshold", sbUpperThreshold );

    sbUpperThreshold->setRange( -1024, 3071 );
    sbUpperThreshold->setValue( drr.upperThreshold() );
    sbUpperThreshold->setSingleStep( 10 );

    connect( sbUpperThreshold, SIGNAL( valueChanged( int ) ), this, SLOT( setUpperThreshold( int ) ) );

 // upper multiplier

    filtering->addRow( "Upper Multiplier", sbUpperMultiplier );

    sbUpperMultiplier->setRange( 0., std::numeric_limits< float >::max() );
    sbUpperMultiplier->setValue( drr.upperMultiplier() );
    sbUpperMultiplier->setDecimals( 2 );
    sbUpperMultiplier->setSingleStep( 0.1 );

    connect( sbUpperMultiplier, SIGNAL( valueChanged( double ) ), this, SLOT( setUpperMultiplier( double ) ) );

 // finish

    layout->addStretch( 1 );
    this->setLayout( layout );
}


void DRRControl::setWaterAttenuation( double muWater )
{
    const float muWaterF = static_cast< float >( muWater );
    if( !base::math::isEqual( muWaterF, drr.waterAttenuation() ) )
    {
        drr.setWaterAttenuation( muWaterF );
        RenderStageControl::invalidate();
    }
}


void DRRControl::setBaseIntensity( double baseIntensity )
{
    const float baseIntensityF = static_cast< float >( baseIntensity );
    if( !base::math::isEqual( baseIntensityF, drr.baseIntensity() ) )
    {
        drr.setBaseIntensity( baseIntensityF );
        RenderStageControl::invalidate();
    }
}


void DRRControl::setLowerThreshold( int lowerThreshold )
{
    const base::HUV lowerThresholdHUV = static_cast< base::HUV >( lowerThreshold );
    if( !base::math::isEqual( lowerThresholdHUV, drr.lowerThreshold() ) )
    {
        drr.setLowerThreshold( lowerThresholdHUV );
        RenderStageControl::invalidate();
    }
}


void DRRControl::setUpperThreshold( int upperThreshold )
{
    const base::HUV upperThresholdHUV = static_cast< base::HUV >( upperThreshold );
    if( !base::math::isEqual( upperThresholdHUV, drr.upperThreshold() ) )
    {
        drr.setUpperThreshold( upperThresholdHUV );
        RenderStageControl::invalidate();
    }
}


void DRRControl::setUpperMultiplier( double upperMultiplier )
{
    const float upperMultiplierF = static_cast< float >( upperMultiplier );
    if( !base::math::isEqual( upperMultiplierF, drr.upperMultiplier() ) )
    {
        drr.setUpperMultiplier( upperMultiplierF );
        RenderStageControl::invalidate();
    }
}



}  // namespace Carna :: qt

}  // namespace Carna

