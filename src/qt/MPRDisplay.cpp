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

#include <Carna/qt/MPRDisplay.h>
#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/helpers/FrameRendererHelper.h>
#include <QVBoxLayout>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPRDisplay :: Details
// ----------------------------------------------------------------------------------

struct MPRDisplay::Details
{
    MPRDisplay& self;
    Details
        ( MPRDisplay& self
        , unsigned int geometryTypeVolumetric
        , unsigned int geometryTypePlanes
        , const std::vector< base::RenderStage* >& extraRenderStages );
    
    Display* const display;
    static Display* createDisplay
        ( unsigned int geometryTypeVolumetric
        , unsigned int geometryTypePlanes
        , const std::vector< base::RenderStage* >& extraRenderStages );
};


MPRDisplay::Details::Details
        ( MPRDisplay& self
        , unsigned int geometryTypeVolumetric
        , unsigned int geometryTypePlanes
        , const std::vector< base::RenderStage* >& extraRenderStages )
    : self( self )
    , display( createDisplay( geometryTypeVolumetric, geometryTypePlanes, extraRenderStages ) )
{
}


Display* MPRDisplay::Details::createDisplay
    ( unsigned int geometryTypeVolumetric
    , unsigned int geometryTypePlanes
    , const std::vector< base::RenderStage* >& extraRenderStages )
{
    FrameRendererFactory* const frFactory = new FrameRendererFactory();
    helpers::FrameRendererHelper< > frHelper( *frFactory );
    frHelper << new presets::CuttingPlanesStage( geometryTypeVolumetric, geometryTypePlanes );
    for( auto rsItr = extraRenderStages.begin(); rsItr != extraRenderStages.end(); ++rsItr )
    {
        frHelper << *rsItr;
    }
    frHelper.commit();
    return new Display( frFactory );
}



// ----------------------------------------------------------------------------------
// MPRDisplay
// ----------------------------------------------------------------------------------

MPRDisplay::MPRDisplay
        ( unsigned int geometryTypeVolumetric
        , unsigned int geometryTypePlanes
        , const std::vector< base::RenderStage* >& extraRenderStages
        , QWidget* parent )
    : QWidget( parent )
    , pimpl( new Details( *this, geometryTypeVolumetric, geometryTypePlanes, extraRenderStages ) )
    , geometryTypeVolumetric( geometryTypeVolumetric )
    , geometryTypePlanes( geometryTypePlanes )
{
    setLayout( new QVBoxLayout() );
    this->layout()->addWidget( pimpl->display );
    this->layout()->setContentsMargins( 0, 0, 0, 0 );
    
    /* Resize this widget to its recommended minimum size if it is a distinct window.
     */
    if( parent == nullptr )
    {
        this->resize( this->minimumSizeHint() );
    }
}


MPRDisplay::~MPRDisplay()
{
}


QSize MPRDisplay::minimumSizeHint() const
{
    return QSize( 400, 400 );
}



}  // namespace Carna :: qt

}  // namespace Carna

