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

FrameRendererFactory::~FrameRendererFactory()
{
    reset();
}


void FrameRendererFactory::reset()
{
    std::for_each( myStages.begin(), myStages.end(), std::default_delete< base::RenderStage >() );
    myStages.clear();
}


base::FrameRenderer* FrameRendererFactory::createRenderer( base::GLContext& glc, unsigned int width, unsigned int height, bool fitSquare )
{
    base::FrameRenderer* const renderer = new base::FrameRenderer( glc, width, height, fitSquare );
    for( auto stageItr = myStages.begin(); stageItr != myStages.end(); ++stageItr )
    {
        renderer->appendStage( *itr );
    }
    myStages.clear();
    return renderer;
}


std::size_t FrameRendererFactory::stages() const
{
    return myStages.size();
}


void FrameRendererFactory::appendStage( RenderStage* stage )
{
    myStages.push_back( stage );
}


void FrameRendererFactory::clearStages()
{
    reset();
}


RenderStage& FrameRendererFactory::stageAt( std::size_t position ) const
{
    CARNA_ASSERT( position < stages() );
    return *myStages[ position ];
}



}  // namespace Carna :: qt

}  // namespace Carna

