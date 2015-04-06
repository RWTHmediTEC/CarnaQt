//! [windowing_includes]
#include <Carna/qt/Application.h>
#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/qt/WindowingControl.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/presets/CameraShowcaseControl.h>
#include <Carna/helpers/FrameRendererHelper.h>
#include <Carna/base/Composition.h>
//! [windowing_includes]
#include <TestScene.h>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

//! [windowing_constants]
const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
const static unsigned int GEOMETRY_TYPE_PLANES     = 1;
//! [windowing_constants]


//! [windowing_main]
int main( int argc, char** argv )
{
    using namespace Carna;
    
    /* Reports exceptions graphically and setups the logger s.t. it works with Qt.
     */
    qt::Application app( argc, argv );
    
    /* Define an arbitrary rendering stages sequence.
     */
    qt::FrameRendererFactory* const frFactory = new qt::FrameRendererFactory();
    presets::CuttingPlanesStage* const planes = new presets::CuttingPlanesStage( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_PLANES );
    helpers::FrameRendererHelper< > frHelper( *frFactory );
    frHelper << planes;
    frHelper.commit();
    
    /* Lets create a predefined window for windowing.
     */
    qt::WindowingControl windowingControl( new qt::WindowingControl::GenericWindowingAdapter< presets::CuttingPlanesStage >( *planes ) );
    
    /* Attach an arbitrary cutting plane to the scene.
     */
    testing::TestScene scene( testing::TestScene::NORMAL_MAP_NOT_REQUIRED );
    scene.root().attachChild( new base::Geometry( GEOMETRY_TYPE_PLANES ) );
    
    /* A display is like the habitat of an 'base::FrameRenderer' object.
     * The display's constructor takes possession of our 'frFactory'.
     * The tag identifies the display within log messages.
     */
    qt::Display display( frFactory );
    
    /* The 'TestScene' object simply holds the root node of the scene and provides
     * access to an arbitrary 'base::Camera' object through its 'cam' method.
     */
    display.setCamera( scene.cam() );
    display.setCameraControl( new base::Composition< base::CameraControl >( new presets::CameraShowcaseControl() ) );
    
    /* Here we actually run the application.
     */
    display.show();
    windowingControl.show();
    return QApplication::exec();
}
//! [windowing_main]

