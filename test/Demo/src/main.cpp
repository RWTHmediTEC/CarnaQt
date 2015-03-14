#include <QApplication>
#include <Carna/qt/Display.h>
#include <Carna/helpers/FrameRendererHelper.h>
#include <Carna/presets/DRRStage.h>
#include <Carna/presets/CameraShowcaseControl.h>
#include <Carna/base/Composition.h>
#include <TestScene.h>
#include <memory>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;


int main( int argc, char** argv )
{
    using namespace Carna;
    QApplication app( argc, argv );
    
    qt::Display display
    (
        [&]( base::FrameRenderer& fr )
        {
            helpers::FrameRendererHelper< > helper( fr );
            helper << new presets::DRRStage( GEOMETRY_TYPE_VOLUMETRIC );
            helper.commit();
        }
    );
    
    testing::TestScene scene;
    display.setCamera( scene.cam() );
    display.setCameraControl( new base::Composition< base::CameraControl >( new presets::CameraShowcaseControl() ) );
    
    display.show();
    return QApplication::exec();
}

