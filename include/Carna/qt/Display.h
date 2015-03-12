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

#ifndef DISPLAY_H_0874895466
#define DISPLAY_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/noncopyable.h>
#include <QGLWidget>
#include <memory>

/** \file   Display.h
  * \brief  Defines \ref Carna::qt::Display.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Display
// ----------------------------------------------------------------------------------

class CARNAQT_LIB Display : public QGLWidget
{

    NON_COPYABLE
    
    struct Details;
    std::unique_ptr< Details > pimpl;

public:

    explicit Display( QWidget* parent = nullptr );
    
    virtual ~Display();
    
    void setCamera( base::Camera& );
    
    base::Camera& camera();
    
    const base::Camera& camera() const;
    
protected:

    /** \brief
      * Creates the renderer that will be responsible for rendering frames within
      * this display for the lifetime of the display.
      *
      * \note
      * You cannot put a frame renderer from one display to another. The reason for
      * this strong coupling is that \c FrameRenderer requires *one* particular GL
      * context, whose lifetime is coupled to the timetime of the hosting
      * \c QGLWidget. So instead of moving the renderer, just put the display to
      * another location in your GUI.
      */
    virtual base::FrameRenderer* createRenderer( base::GLContext&, unsigned int width, unsigned int height ) = 0;
    
    virtual void initializeGL() override;
    
    virtual void resizeGL( int w, int h ) override;
    
    virtual void paintGL() override;

}; // Display



}  // namespace Carna :: qt

}  // namespace Carna

#endif // DISPLAY_H_0874895466

