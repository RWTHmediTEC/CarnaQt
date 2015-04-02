//! [quick_start_includes]
#include <Carna/qt/Application.h>
#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/helpers/FrameRendererHelper.h>
#include <Carna/presets/DRRStage.h>
#include <Carna/presets/CameraShowcaseControl.h>
#include <Carna/base/Composition.h>
//! [quick_start_includes]
#include <TestScene.h>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

//! [quick_start_constants]
const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
//! [quick_start_constants]


//! [quick_start_main]
int main( int argc, char** argv )
{
    using namespace Carna;
    
    /* Reports exceptions graphically and setups the logger s.t. it works with Qt.
     */
    qt::Application app( argc, argv );
    
    /* Define an arbitrary rendering stages sequence.
     */
    qt::FrameRendererFactory* const frFactory = new qt::FrameRendererFactory();
    helpers::FrameRendererHelper< > frHelper( *frFactory );
    frHelper << new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
    frHelper.commit();
    
    /* A display is like the habitat of an 'base::FrameRenderer' object.
     * The display's constructor takes possession of our 'frFactory'.
     * The tag identifies the display within log messages.
     */
    qt::Display display( frFactory );
    display.setLogTag( "DRR" );
    
    /* The 'TestScene' object simply holds the root node of the scene and provides
     * access to an arbitrary 'base::Camera' object through its 'cam' method.
     */
    testing::TestScene scene;
    display.setCamera( scene.cam() );
    display.setCameraControl( new base::Composition< base::CameraControl >( new presets::CameraShowcaseControl() ) );
    
    /* Here we actually run the application.
     */
    display.show();
    return QApplication::exec();
}
//! [quick_start_main]

