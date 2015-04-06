//! [mpr_includes]
#include <Carna/qt/Application.h>
#include <Carna/qt/MPR.h>
#include <Carna/qt/MPRDisplay.h>
#include <Carna/base/Color.h>
//! [mpr_includes]
#include <TestScene.h>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

//! [mpr_constants]
const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
const static unsigned int GEOMETRY_TYPE_PLANES     = 1;
//! [mpr_constants]


//! [mpr_main]
int main( int argc, char** argv )
{
    using namespace Carna;
    testing::TestScene scene( testing::TestScene::NORMAL_MAP_NOT_REQUIRED );
    
    /* Reports exceptions graphically and setups the logger s.t. it works with Qt.
     */
    qt::Application app( argc, argv );
    
    /* Create MPR displays.
     */
    const qt::MPRDisplay::Parameters params( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_PLANES );
    
    qt::MPRDisplay front( params );
    qt::MPRDisplay left ( params );
    qt::MPRDisplay top  ( params );
    
    left.setRotation( qt::MPRDisplay::ROTATION_LEFT );
    top .setRotation( qt::MPRDisplay::ROTATION_TOP  );
    
    /* Lets name the displays and configure distinct colors for convenience.
     */
    front.setWindowTitle( "Front" );
    left .setWindowTitle( "Left" );
    top  .setWindowTitle( "Top" );
    
    front.setPlaneColor( base::Color::BLUE  );
    left .setPlaneColor( base::Color::RED   );
    top  .setPlaneColor( base::Color::GREEN );
    
    /* We may also supply log tags to distinguish debug messages.
     */
    front.setLogTag( front.windowTitle().toStdString() );
    left .setLogTag( left .windowTitle().toStdString() );
    top  .setLogTag( top  .windowTitle().toStdString() );
    
    /* Interconnect the displays.
     */
    qt::MPR mpr( GEOMETRY_TYPE_VOLUMETRIC );
    front.setMPR( mpr );
    left .setMPR( mpr );
    top  .setMPR( mpr );
    
    /* The 'TestScene' object simply holds the root node of the scene.
     */
    mpr.setRoot( scene.root() );
    
    /* Here we actually run the application.
     */
    front.show();
    left .show();
    top  .show();
    return QApplication::exec();
}
//! [mpr_main]

