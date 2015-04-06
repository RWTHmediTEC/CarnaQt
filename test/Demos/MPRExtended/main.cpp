//! [mpr_includes]
#include <Carna/qt/Application.h>
#include <Carna/qt/MPR.h>
#include <Carna/qt/MPRDisplay.h>
#include <Carna/presets/OpaqueRenderingStage.h>
#include <Carna/presets/MeshColorCodingStage.h>
#include <Carna/presets/OccludedRenderingStage.h>
#include <Carna/helpers/FrameRendererHelper.h>
#include <Carna/helpers/PointMarkerHelper.h>
#include <Carna/base/ManagedMesh.h>
#include <Carna/base/MeshFactory.h>
#include <Carna/base/Material.h>
#include <Carna/base/Color.h>
//! [mpr_includes]
#include <TestScene.h>



// ----------------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------------

//! [mpr_constants]
const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;
const static unsigned int GEOMETRY_TYPE_PLANES     = 1;
const static unsigned int GEOMETRY_TYPE_OPAQUE     = 2;
//! [mpr_constants]


//! [mpr_extra_stages]
using namespace Carna;
struct MPRFactory : public qt::MPRDisplay::Factory
{
    MPRFactory( const qt::MPRDisplay::Parameters& params )
        : qt::MPRDisplay::Factory( params )
    {
    }
    
    virtual void addExtraStages( base::RenderStageSequence& to ) const override
    {
        helpers::FrameRendererHelper< > rsHelper( to );
        rsHelper << new presets::OpaqueRenderingStage( GEOMETRY_TYPE_OPAQUE );
        rsHelper << new presets::MeshColorCodingStage();
        rsHelper << new presets::OccludedRenderingStage();
        
        /* We are not allowed to remove stages already added:
         */
        rsHelper.commit( false );
    }
};
//! [mpr_extra_stages]


//! [mpr_main]
int main( int argc, char** argv )
{
    testing::TestScene scene( testing::TestScene::NORMAL_MAP_NOT_REQUIRED );
    
    /* Reports exceptions graphically and setups the logger s.t. it works with Qt.
     */
    qt::Application app( argc, argv );
    
    /* Create MPR displays.
     */
    const qt::MPRDisplay::Parameters params( GEOMETRY_TYPE_VOLUMETRIC, GEOMETRY_TYPE_PLANES );
    const MPRFactory mprFactory( params );
    
    qt::MPRDisplay front( mprFactory );
    qt::MPRDisplay left ( mprFactory );
    qt::MPRDisplay top  ( mprFactory );
    
    left.setRotation( qt::MPRDisplay::ROTATION_LEFT );
    top .setRotation( qt::MPRDisplay::ROTATION_TOP  );
    
    /* Add some opaque meshes to the scene.
     */
    helpers::PointMarkerHelper markers( GEOMETRY_TYPE_OPAQUE );
    base::Geometry* const m1 = markers.createPointMarker();
    base::Geometry* const m2 = markers.createPointMarker();
    m1->localTransform = base::math::translation4f( -50, -50, -50 );
    m2->localTransform = base::math::translation4f( +50, +50, +50 );
    scene.root().attachChild( m1 );
    scene.root().attachChild( m2 );
    
    base::ManagedMeshBase& boxMesh = base::MeshFactory< base::VertexBase >::createBox( 20, 20, 20 );
    base::Material& boxMaterial = base::Material::create( "unshaded" );
    boxMaterial.setParameter( "color", base::math::Vector4f( 1, 1, 0, 1 ) );
    base::Geometry* const box = new base::Geometry( GEOMETRY_TYPE_OPAQUE );
    box->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MESH, boxMesh );
    box->putFeature( presets::OpaqueRenderingStage::ROLE_DEFAULT_MATERIAL, boxMaterial );
    box->localTransform = base::math::translation4f( 0, 0, 80 );
    boxMesh.release();
    boxMaterial.release();
    scene.root().attachChild( box );
    
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

