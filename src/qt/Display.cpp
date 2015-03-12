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

#include <Carna/qt/Display.h>
#include <Carna/base/FrameRenderer.h>
#include <Carna/base/GLContext.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Log.h>
#include <QGLContext>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Display :: Details
// ----------------------------------------------------------------------------------

struct Display::Details
{
    Details();
    
    typedef base::QGLContextAdapter< QGLContext > GLContext;

    std::unique_ptr< GLContext > glc;
    std::unique_ptr< base::FrameRenderer > renderer;
    
    base::Camera* cam;
    base::Node* root;
};


Display::Details::Details()
    : cam( nullptr )
    , root( nullptr )
{
}



// ----------------------------------------------------------------------------------
// Display
// ----------------------------------------------------------------------------------

Display::Display( QWidget* parent )
    : QGLWidget( parent )
    , pimpl( new Details() )
{
}


Display::~Display()
{
    if( pimpl->glc.get() != nullptr )
    {
        pimpl->glc->makeActive();
    }
}


void Display::setCamera( base::Camera& cam )
{
    pimpl->cam = &cam;
    
    /* We will search the root node the first time we render the frame.
     */
    pimpl->root = nullptr;
}


base::Camera& Display::camera()
{
    return *pimpl->cam;
}


const base::Camera& Display::camera() const
{
    return *pimpl->cam;
}


void Display::initializeGL()
{
    CARNA_ASSERT( pimpl->glc.get() == nullptr );
    pimpl->glc.reset( new Details::GLContext() );
}


void Display::resizeGL( int w, int h )
{
    CARNA_ASSERT( pimpl->glc.get() != nullptr );
    const unsigned int width  = static_cast< unsigned int >( w );
    const unsigned int height = static_cast< unsigned int >( h );
    if( pimpl->renderer == nullptr )
    {
        base::FrameRenderer* const renderer = createRenderer( *pimpl->glc, width, height );
        CARNA_ASSERT( renderer != nullptr );
        pimpl->renderer.reset( renderer );
    }
    else
    {
        pimpl->renderer->reshape( width, height );
    }
}


void Display::paintGL()
{
    CARNA_ASSERT( pimpl->renderer.get() != nullptr );
    
    if( pimpl->cam == nullptr )
    {
        base::Log::instance().record( base::Log::debug, "Display has no camera but should render." );
    }
    else
    {
        if( pimpl->root == nullptr )
        {
            pimpl->root = &pimpl->cam->findRoot();
        }
        pimpl->renderer->render( *pimpl->cam, *pimpl->root );
    }
}



}  // namespace Carna :: qt

}  // namespace Carna

