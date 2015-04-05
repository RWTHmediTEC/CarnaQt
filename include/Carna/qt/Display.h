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
  * Provides a `base::GLContext` object and encapsulates a `base::FrameRenderer` that
  * it uses for rendering itself.
  *
  * If a \ref setCameraControl "camera control" is supplied, this class also updates
  * the camera's state based on user input:
  *
  *   - Dragging the mouse while holding its primary button rotates the camera.
  *   - Scrolling the mouse wheel shifts the camera along its z-axis.
  *
  * You must \ref setCamera "specify which camera is to be used" before rendering.
  *
  * This class also implements drag-&-drop behaviour for mesh-typed geometry. This
  * functionality is enabled if an instance of `presets::MeshColorCodingStage` is
  * found within the rendering stages sequence.
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
    
    /** \brief
      * Holds the default scene units per scrolled wheel distance ratio used for
      * camera movement.
      */
    const static float DEFAULT_AXIAL_MOVEMENT_SPEED;
    
    /** \brief
      * Defines how the root viewport is embedded into the rendered frame.
      *
      * Note that for `fitSquare` the viewport's side lengths' ratio is \f$1\f$
      * constantly. When `fitFrame` is used, the camera's projection matrix must be
      * updated each time the frame's side lengths' ratio changes. This class takes
      * care of this if a \ref setProjectionControl "projection control" is supplied.
      * The default option `fitAuto` chooses automatically between `fitSquare` and
      * `fitFrame`, based on whether such a control is available or not.
      */
    enum ViewportMode
    {
        fitSquare,  ///< Fits a maximum-sized square-shaped viewport, that is centered within the frame.
        fitFrame,   ///< Fits the viewport s.t. it covers the whole frame.
        fitAuto     ///< Chooses between `fitSquare` and `fitFrame` automatically.
    };

    /** \brief
      * Instantiates and takes possession of \a rendererFactory.
      */
    explicit Display( FrameRendererFactory* rendererFactory, QWidget* parent = nullptr );
    
    /** \brief
      * Deletes.
      */
    virtual ~Display();
    
    /** \brief
      * Posts an `updateGL` invocation to the message queue if such hasn't been
      * posted yet.
      */
    void invalidate();
    
    /** \brief Sets how the root viewport is to be embedded into the rendered frame.
      */
    void setViewportMode( ViewportMode );
    
    /** \brief
      * Sets the radians per pixel ratio used for camera rotation.
      */
    void setRotationSpeed( float radiansPerPixel );
    
    /** \brief
      * Sets the scene units per scrolled wheel distance ratio used for camera
      * movement.
      */
    void setAxialMovementSpeed( float axialMovementSpeed );
    
    /** \brief
      * Denotes that \a cam is to be used for future rendering.
      * \post `hasCamera() == true`
      */
    void setCamera( base::Camera& cam );
    
    /** \brief
      * Tells whether \ref setCamera has been invoked previously.
      */
    bool hasCamera() const;
    
    /** \brief
      * References the camera that is currently being used for rendering.
      * \pre `hasCamera() == true`
      */
    base::Camera& camera();
    
    /** \overload
      */
    const base::Camera& camera() const;
    
    /** \brief
      * Maps user input to camera control instructions.
      *
      * \param camControl the object that the camera control is to be delegated to.
      */
    void setCameraControl( base::Association< base::CameraControl >* camControl );
    
    /** \brief
      * Tells whether this instance does control the camera through user input.
      */
    bool hasCameraControl() const;
    
    /** \brief
      * References the object that the camera control is delegated to.
      * \pre `hasCameraControl() == true`
      */
    base::CameraControl& cameraControl();
    
    /** \overload
      */
    const base::CameraControl& cameraControl() const;
    
    /** \brief
      * Sets the object that is to be triggered when the camera's projection matrix
      * needs an update due to changed viewport's side lenghts to \a projControl.
      *
      * \param projControl might be `nullptr`.
      */
    void setProjectionControl( base::Association< base::ProjectionControl >* projControl );
    
    /** \brief
      * Tells whether \ref setProjectionControl "an object has been supplied" that is
      * to be triggered when the camera's projection matrix needs an update due to
      * changed viewport's side lenghts
      */
    bool hasProjectionControl() const;
    
    /** \brief
      * References the object that is to be triggered when the camera's projection
      * matrix needs an update due to changed viewport's side lenghts.
      * \pre `hasProjectionControl() == true`
      */
    base::ProjectionControl& projectionControl();
    
    /** \overload
      */
    const base::ProjectionControl& projectionControl() const;
    
    /** \brief
      * Queries the \ref projectionControl for an update the next time a frame is
      * rendered. Does nothing if no `base::ProjectionControl` is supplied.
      *
      * Also \ref invalidate "invalidates" this `%Display`.
      */
    void updateProjection();
    
    /** \brief
      * Tells whether the frame renderer already has been loaded.
      *
      * The frame renderer will be loaded when this `%Display` widget instance
      * becomes visible for the first time.
      */
    bool hasRenderer() const;
    
    /** \brief
      * References the frame renderer.
      * \pre `hasRenderer() == true`
      */
    base::FrameRenderer& renderer();
    
    /** \overload
      */
    const base::FrameRenderer& renderer() const;
    
    /** \brief
      * Denotes that \a tag is to be used within logged messages to identify this
      * `%Display` instance.
      */
    void setLogTag( const std::string& tag );
    
    /** \brief
      * Tells which tag annotates log messages to identify this `%Display` instance.
      */
    const std::string& logTag() const;
    
    /** \brief
      * Finds and returns the `%Display` instance that \a renderer belongs to in
      * \f$\mathcal O\left(\log n\right)\f$. Returns
      * `base::Aggregation<%Display>::%NULL_PTR` if no such `%Display` is found.
      */
    static base::Aggregation< Display > byRenderer( const base::FrameRenderer& renderer );
    
protected:
    
    /** \brief
      * Loads the `base::GLContext` wrapper.
      */
    virtual void initializeGL() override;
    
    /** \brief
      * Loads or updates the `base::FrameRenderer`.
      */
    virtual void resizeGL( int w, int h ) override;
    
    /** \brief
      * Triggers the `base::FrameRenderer`.
      */
    virtual void paintGL() override;
    
    /** \brief
      * Starts mouse interaction.
      */
    virtual void mousePressEvent( QMouseEvent* ev ) override;
    
    /** \brief
      * Processes mouse interaction.
      */
    virtual void mouseMoveEvent( QMouseEvent* ev ) override;
    
    /** \brief
      * Finishes mouse interaction.
      */
    virtual void mouseReleaseEvent( QMouseEvent* ev ) override;
    
    /** \brief
      * Processes mouse interaction.
      */
    virtual void wheelEvent( QWheelEvent* ev ) override;

}; // Display



}  // namespace Carna :: qt

}  // namespace Carna

#endif // DISPLAY_H_0874895466

