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
    qt::MPRDisplay front;
    qt::MPRDisplay left;
    qt::MPRDisplay top;
    left.setRotation( base::math::rotation3f( 0, 1, 0, base::math::deg2rad( +90 ) ) );
    top .setRotation( base::math::rotation3f( 1, 0, 0, base::math::deg2rad( -90 ) ) );
    
    /* Lets the name displays for convenience.
     */
    front.setWindowTitle( "Front" );
    left .setWindowTitle( "Left" );
    top  .setWindowTitle( "Top" );
    
    /* The 'TestScene' object simply holds the root node of the scene.
     */
    testing::TestScene scene( testing::TestScene::NORMAL_MAP_NOT_REQUIRED );
    front.setRoot( scene.root() );
    left .setRoot( scene.root() );
    top  .setRoot( scene.root() );
    
    /* Here we actually run the application.
     */
    front.show();
    left .show();
    top  .show();
    return QApplication::exec();
}
//! [mpr_main]

