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

#include <Carna/qt/VolumeRenderingControl.h>
#include <Carna/presets/VolumeRenderingStage.h>
#include <QSpinBox>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// VolumeRenderingControl
// ----------------------------------------------------------------------------------

VolumeRenderingControl::VolumeRenderingControl( presets::VolumeRenderingStage& stage, QWidget* parent )
    : QWidget( parent )
    , RenderStageControl( stage )
    , stage( stage )
    , sbSampleRate( new QSpinBox() )
{
    sbSampleRate->setMinimum( 10 );
    sbSampleRate->setMaximum( 10000 );
    sbSampleRate->setSingleStep( 10 );
    sbSampleRate->setValue( stage.sampleRate() );
    
    connect( sbSampleRate, SIGNAL( valueChanged( int ) ), this, SLOT( setSampleRate( int ) ) );
}


VolumeRenderingControl::~VolumeRenderingControl()
{
}


void VolumeRenderingControl::setSampleRate( int samplesPerPixel )
{
    samplesPerPixel = base::math::clamp< int >( samplesPerPixel, sbSampleRate->minimum(), sbSampleRate->maximum() );
    if( samplesPerPixel != stage.sampleRate() )
    {
        stage.setSampleRate( samplesPerPixel );
        RenderStageControl::invalidate();
    }
}



}  // namespace Carna :: qt

}  // namespace Carna
