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

#include <Carna/qt/RenderStageControl.h>
#include <Carna/qt/RenderStageControlDetails.h>
#include <Carna/qt/Display.h>
#include <QTimer>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// RenderStageControl :: Details
// ----------------------------------------------------------------------------------

RenderStageControl::Details::Details( RenderStageControl& self )
    : self( self )
    , updateScheduled( false )
{
}


void RenderStageControl::Details::updateDisplay()
{
    if( self.renderStage.isInitialized() )
    {
        const base::FrameRenderer& renderer = self.renderStage.renderer();
        const auto display = Display::byRenderer( renderer );
        if( display )
        {
            display->updateGL();
        }
        updateScheduled = false;
    }
}



// ----------------------------------------------------------------------------------
// RenderStageControl
// ----------------------------------------------------------------------------------

RenderStageControl::RenderStageControl( base::RenderStage& rs )
    : pimpl( new Details( *this ) )
    , renderStage( rs )
{
}


void RenderStageControl::invalidate()
{
    if( !pimpl->updateScheduled )
    {
        pimpl->updateScheduled = true;
        QTimer::singleShot( 0, pimpl.get(), SLOT( updateDisplay() ) );
    }
}



}  // namespace Carna :: qt

}  // namespace Carna

