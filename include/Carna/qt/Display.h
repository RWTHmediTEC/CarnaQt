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
#include <Carna/base/Association.h>
#include <QGLWidget>
#include <memory>

class QMouseEvent;
class QWheelEvent;

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

/** \brief
  * Encapsulates a `base::FrameRenderer` and a `base::GLContext`.
  *
  * \note
  * You cannot put a frame renderer from one display to another. The reason for this
  * strong coupling is that `base::FrameRenderer` requires *one* particular OpenGL
  * context, whose lifetime is coupled to the timetime of the hosting \c QGLWidget.
  * So instead of moving the renderer, just put the display to another location in
  * your GUI.
  *
  * \author Leonid Kostrykin
  * \date   21.2.15 - 2.4.15
  */
class CARNAQT_LIB Display : public QGLWidget
{

    NON_COPYABLE
    
    struct Details;
    std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Holds the default radians per pixel ratio used for camera rotation.
      */
    const static float DEFAULT_ROTATION_SPEED;
    
    const static float DEFAULT_AXIAL_MOVEMENT_SPEED;
    
    enum ViewportMode
    {
        fitSquare,
        fitFrame,
        fitAuto
    };

    explicit Display( FrameRendererFactory* rendererFactory, QWidget* parent = nullptr );
    
    virtual ~Display();
    
    void setViewportMode( ViewportMode );
    
    void setRotationSpeed( float radiansPerPixel );
    
    void setAxialMovementSpeed( float axialMovementSpeed );
    
    void setCamera( base::Camera& );
    
    bool hasCamera() const;
    
    base::Camera& camera();
    
    const base::Camera& camera() const;
    
    void setCameraControl( base::Association< base::CameraControl >* camControl );
    
    bool hasCameraControl() const;
    
    base::CameraControl& cameraControl();
    
    const base::CameraControl& cameraControl() const;
    
    void setProjectionControl( base::Association< base::ProjectionControl >* projControl );
    
    bool hasProjectionControl() const;
    
    base::ProjectionControl& projectionControl();
    
    const base::ProjectionControl& projectionControl() const;
    
    bool hasRenderer() const;
    
    base::FrameRenderer& renderer();
    
    const base::FrameRenderer& renderer() const;
    
protected:
    
    virtual void initializeGL() override;
    
    virtual void resizeGL( int w, int h ) override;
    
    virtual void paintGL() override;
    
    virtual void mousePressEvent( QMouseEvent* ev ) override;
    
    virtual void mouseMoveEvent( QMouseEvent* ev ) override;
    
    virtual void mouseReleaseEvent( QMouseEvent* ev ) override;
    
    virtual void wheelEvent( QWheelEvent* ev ) override;

}; // Display



}  // namespace Carna :: qt

}  // namespace Carna

#endif // DISPLAY_H_0874895466

