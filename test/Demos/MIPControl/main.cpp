//! [mip_includes]
#include <Carna/qt/Application.h>
#include <Carna/qt/Display.h>
#include <Carna/qt/FrameRendererFactory.h>
#include <Carna/qt/MIPControl.h>
#include <Carna/presets/MIPStage.h>
#include <Carna/presets/MIPLayer.h>
#include <Carna/presets/CameraShowcaseControl.h>
#include <Carna/helpers/FrameRendererHelper.h>
#include <Carna/base/Composition.h>
#include <Carna/base/Color.h>
//! [mip_includes]
#include <TestScene.h>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

//! [mip_constants]
const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
//! [mip_constants]


//! [mip_main]
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
    frHelper << new presets::MIPStage( GEOMETRY_TYPE_VOLUMETRIC );
    frHelper.commit();
    
    /* Pre-configure the MIP rendering stage.
     */
    presets::MIPStage& mip = *frFactory->findStage< presets::MIPStage >();
    mip.appendLayer( new presets::MIPLayer( -1024, 3071, base::Color::WHITE ) );
    
    /* Lets create a predefined window for adjusting the MIP parameters.
     */
    qt::MIPControl mipControl( mip );
    
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
    mipControl.show();
    return QApplication::exec();
}
//! [mip_main]

