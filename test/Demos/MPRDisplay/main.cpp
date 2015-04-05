//! [mpr_includes]
#include <Carna/qt/Application.h>
#include <Carna/qt/MPRDisplay.h>
//! [mpr_includes]
#include <TestScene.h>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

//! [mpr_main]
int main( int argc, char** argv )
{
    using namespace Carna;
    
    /* Reports exceptions graphically and setups the logger s.t. it works with Qt.
     */
    qt::Application app( argc, argv );
    
    /* Create MPR.
     */
    qt::MPRDisplay display;
    
    /* The 'TestScene' object simply holds the root node of the scene.
     */
    testing::TestScene scene( testing::TestScene::NORMAL_MAP_NOT_REQUIRED );
    display.setRoot( scene.root() );
    
    /* Here we actually run the application.
     */
    display.show();
    return QApplication::exec();
}
//! [mpr_main]

