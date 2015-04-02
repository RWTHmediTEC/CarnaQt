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

#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/RenderStage.h>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// FrameRendererFactory
// ----------------------------------------------------------------------------------

base::FrameRenderer* FrameRendererFactory::createRenderer( base::GLContext& glc, unsigned int width, unsigned int height, bool fitSquare )
{
    base::FrameRenderer* const renderer = new base::FrameRenderer( glc, width, height, fitSquare );
    for( std::size_t stageIdx = 0; stageIdx < stages(); ++stageIdx )
    {
        base::RenderStage& stage = stageAt( stageIdx );
        renderer->appendStage( &stage );
    }
    releaseStages();
    return renderer;
}



}  // namespace Carna :: qt

}  // namespace Carna

