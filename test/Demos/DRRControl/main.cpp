//! [drr_includes]
#include <Carna/qt/Application.h>
#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/qt/DRRControl.h>
#include <Carna/presets/DRRStage.h>
#include <Carna/presets/CameraShowcaseControl.h>
#include <Carna/helpers/FrameRendererHelper.h>
#include <Carna/base/Composition.h>
//! [drr_includes]
#include <TestScene.h>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

//! [drr_constants]
const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
//! [drr_constants]


//! [drr_main]
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
    
    /* Lets create a predefined window for adjusting the DRR parameters.
     */
    qt::DRRControl drrControl( *frFactory->findStage< presets::DRRStage >() );
    
    /* A display is like the habitat of an 'base::FrameRenderer' object.
     * The display's constructor takes possession of our 'frFactory'.
     * The tag identifies the display within log messages.
     */
    qt::Display display( frFactory );
    
    /* The 'TestScene' object simply holds the root node of the scene and provides
     * access to an arbitrary 'base::Camera' object through its 'cam' method.
     */
    testing::TestScene scene( testing::TestScene::NORMAL_MAP_NOT_REQUIRED );
    display.setCamera( scene.cam() );
    display.setCameraControl( new base::Composition< base::CameraControl >( new presets::CameraShowcaseControl() ) );
    
    /* Here we actually run the application.
     */
    display.show();
    drrControl.show();
    return QApplication::exec();
}
//! [drr_main]

