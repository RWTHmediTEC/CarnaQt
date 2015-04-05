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
    
    /* Reports exceptions graphically and setups the logger s.t. it works with Qt.
     */
    qt::Application app( argc, argv );
    
    /* Create MPR displays.
     */
    const qt::MPRDisplay::Parameters params( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_PLANES );
    qt::MPRDisplay front( params );
    qt::MPRDisplay left ( params );
    qt::MPRDisplay top  ( params );
    left.setRotation( base::math::rotation3f( 0, 1, 0, base::math::deg2rad( +90 ) ) );
    top .setRotation( base::math::rotation3f( 1, 0, 0, base::math::deg2rad( -90 ) ) );
    
    /* Lets name the displays for convenience.
     */
    front.setWindowTitle( "Front" );
    left .setWindowTitle( "Left" );
    top  .setWindowTitle( "Top" );
    
    /* Interconnect the displays.
     */
    qt::MPR mpr( GEOMETRY_TYPE_VOLUMETRIC );
    front.setMPR( mpr, base::Color::BLUE  );
    left .setMPR( mpr, base::Color::RED   );
    top  .setMPR( mpr, base::Color::GREEN );
    
    /* The 'TestScene' object simply holds the root node of the scene.
     */
    testing::TestScene scene( testing::TestScene::NORMAL_MAP_NOT_REQUIRED );
    mpr.setRoot( scene.root() );
    
    /* Here we actually run the application.
     */
    front.show();
    left .show();
    top  .show();
    return QApplication::exec();
}
//! [mpr_main]

