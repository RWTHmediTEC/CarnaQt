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
#include <Carna/base/Color.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/RenderStageListener.h>
#include <Carna/presets/DRRStage.h>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QFormLayout>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// DRRControl :: Details
// ----------------------------------------------------------------------------------

struct DRRControl::Details : public base::RenderStageListener
{
    Details( DRRControl& self );
    DRRControl& self;
    base::FrameRenderer* renderer;
    bool isRenderStageListener;
    
    void updateRendererBackgroundColor() const;
    void removeRenderStageListener();
    
    QDoubleSpinBox* const sbWaterAttenuation;
    QDoubleSpinBox* const sbBaseIntensity;
    QSpinBox      * const sbLowerThreshold;
    QSpinBox      * const sbUpperThreshold;
    QDoubleSpinBox* const sbUpperMultiplier;
    QCheckBox     * const cbRenderingInverse;
    
    virtual void onRenderStageInitialized( base::RenderStage& rs ) override;
    virtual void onRenderStageDelete( base::RenderStage& rs ) override;
};


DRRControl::Details::Details( DRRControl& self )
    : self( self )
    , renderer( nullptr )
    , isRenderStageListener( false )
    , sbWaterAttenuation( new QDoubleSpinBox() )
    , sbBaseIntensity      ( new QDoubleSpinBox() )
    , sbLowerThreshold  ( new QSpinBox() )
    , sbUpperThreshold  ( new QSpinBox() )
    , sbUpperMultiplier ( new QDoubleSpinBox() )
    , cbRenderingInverse( new QCheckBox() )
{
}


void DRRControl::Details::updateRendererBackgroundColor() const
{
    if( renderer != nullptr )
    {
        renderer->setBackgroundColor( self.drr.isRenderingInverse() ? base::Color::WHITE_NO_ALPHA : base::Color::BLACK_NO_ALPHA );
    }
}


void DRRControl::Details::removeRenderStageListener()
{
    if( isRenderStageListener )
    {
        self.drr.removeRenderStageListener( *this );
        isRenderStageListener = false;
    }
}


void DRRControl::Details::onRenderStageInitialized( base::RenderStage& rs )
{
    CARNA_ASSERT( isRenderStageListener );
    renderer = &rs.renderer();
    updateRendererBackgroundColor();
    removeRenderStageListener();
}


void DRRControl::Details::onRenderStageDelete( base::RenderStage& rs )
{
    CARNA_ASSERT( isRenderStageListener );
    isRenderStageListener = false;
}



// ----------------------------------------------------------------------------------
// DRRControl
// ----------------------------------------------------------------------------------

DRRControl::DRRControl( presets::DRRStage& drr, QWidget* parent )
    : VolumeRenderingControl( drr, parent )
    , pimpl( new Details( *this ) )
    , drr( drr )
{
    QVBoxLayout* const layout = new QVBoxLayout();

    /* Configure 'general parameters'.
     */
    ExpandableGroupBox* const gbDRR = new ExpandableGroupBox( "General", true );
    QFormLayout* const drrParams = new QFormLayout();
    gbDRR->child()->setLayout( drrParams );
    gbDRR->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( gbDRR );

    /* Configure 'sample rate'.
     */
    drrParams->addRow( "Sample Rate:", sbSampleRate );

    /* Configure 'water attenuation'.
     */
    drrParams->addRow( "Water Attenuation:", pimpl->sbWaterAttenuation );

    pimpl->sbWaterAttenuation->setRange( 1e-5, std::numeric_limits< float >::max() );
    pimpl->sbWaterAttenuation->setDecimals( 5 );
    pimpl->sbWaterAttenuation->setValue( drr.waterAttenuation() );
    pimpl->sbWaterAttenuation->setSingleStep( 0.001 );

    connect( pimpl->sbWaterAttenuation, SIGNAL( valueChanged( double ) ), this, SLOT( setWaterAttenuation( double ) ) );

    /* Configure 'base intensity'.
     */
    drrParams->addRow( "Intensity:", pimpl->sbBaseIntensity );

    pimpl->sbBaseIntensity->setRange( 0, std::numeric_limits< float >::max() );
    pimpl->sbBaseIntensity->setValue( drr.baseIntensity() );
    pimpl->sbBaseIntensity->setDecimals( 2 );
    pimpl->sbBaseIntensity->setSingleStep( 0.01 );

    connect( pimpl->sbBaseIntensity, SIGNAL( valueChanged( double ) ), this, SLOT( setBaseIntensity( double ) ) );

    /* Configure 'inverse rendering'.
     */
    drrParams->addRow( pimpl->cbRenderingInverse );

    pimpl->cbRenderingInverse->setText( "Inverse Brightness" );
    pimpl->cbRenderingInverse->setChecked( drr.isRenderingInverse() ? Qt::Checked : Qt::Unchecked );

    connect( pimpl->cbRenderingInverse, SIGNAL( stateChanged( int ) ), this, SLOT( setRenderingInverse( int ) ) );

    /* Configure 'threshold filtering / bone enhancement'.
     */
    ExpandableGroupBox* const gbFiltering = new ExpandableGroupBox( "Threshold Filtering / Bone Enhancement", true );
    QFormLayout* const filtering = new QFormLayout();
    gbFiltering->child()->setLayout( filtering );
    gbFiltering->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget( gbFiltering );

    /* Configure 'lower threshold'.
     */
    filtering->addRow( "Lower Threshold:", pimpl->sbLowerThreshold );

    pimpl->sbLowerThreshold->setRange( -1024, 3071 );
    pimpl->sbLowerThreshold->setValue( drr.lowerThreshold() );
    pimpl->sbLowerThreshold->setSingleStep( 10 );

    connect( pimpl->sbLowerThreshold, SIGNAL( valueChanged( int ) ), this, SLOT( setLowerThreshold( int ) ) );

    /* Configure 'upper threshold'.
     */
    filtering->addRow( "Upper Threshold:", pimpl->sbUpperThreshold );

    pimpl->sbUpperThreshold->setRange( -1024, 3071 );
    pimpl->sbUpperThreshold->setValue( drr.upperThreshold() );
    pimpl->sbUpperThreshold->setSingleStep( 10 );

    connect( pimpl->sbUpperThreshold, SIGNAL( valueChanged( int ) ), this, SLOT( setUpperThreshold( int ) ) );

    /* Configure 'upper multiplier'.
     */
    filtering->addRow( "Upper Multiplier:", pimpl->sbUpperMultiplier );

    pimpl->sbUpperMultiplier->setRange( 0., std::numeric_limits< float >::max() );
    pimpl->sbUpperMultiplier->setValue( drr.upperMultiplier() );
    pimpl->sbUpperMultiplier->setDecimals( 2 );
    pimpl->sbUpperMultiplier->setSingleStep( 0.1 );

    connect( pimpl->sbUpperMultiplier, SIGNAL( valueChanged( double ) ), this, SLOT( setUpperMultiplier( double ) ) );

    /* Finish setup.
     */
    layout->addStretch( 1 );
    this->setLayout( layout );
    if( !drr.isInitialized() )
    {
        /* Get notified when stage initializes.
         */
        drr.addRenderStageListener( *pimpl );
        pimpl->isRenderStageListener = true;
    }
    else
    {
        /* Stage is initialized, we can access its renderer immediately.
         */
        pimpl->renderer = &drr.renderer();
        pimpl->updateRendererBackgroundColor();
        pimpl->isRenderStageListener = false;
    }
}


DRRControl::~DRRControl()
{
    pimpl->removeRenderStageListener();
}


void DRRControl::setWaterAttenuation( double muWater )
{
    const float muWaterF = static_cast< float >( muWater );
    if( std::abs( muWaterF - drr.waterAttenuation() ) > 1e-6f )
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


void DRRControl::setRenderingInverse( int renderingInverse )
{
    bool renderingInverseB = renderingInverse == Qt::Checked;
    if( renderingInverseB != drr.isRenderingInverse() )
    {
        drr.setRenderingInverse( renderingInverseB );
        pimpl->updateRendererBackgroundColor();
        RenderStageControl::invalidate();
    }
}



}  // namespace Carna :: qt

}  // namespace Carna

