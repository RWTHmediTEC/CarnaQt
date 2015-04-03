/*
 *  Copyright (C) 2010 - 2015 Leonid Kostrykin
 *
 *  Chair of Medical Engineering (mediTEC)
 *  RWTH Aachen University
 *  Pauwelsstr. 20
 *  52074 Aachen
 *  Germany
 *
 */

#include <TestScene.h>
#include <HUGZSceneFactory.h>
#include <Carna/helpers/VolumeGridHelper.h>
#include <Carna/base/Node.h>
#include <Carna/base/math.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Geometry.h>
#include <string>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestScene :: Details
// ----------------------------------------------------------------------------------

struct TestScene::Details
{
    Details( TestScene& self, helpers::VolumeGridHelperBase* gridHelper, const base::math::Vector3f& spacing );
    static Details* create( TestScene& self, bool provideNormals );
    
    const std::unique_ptr< helpers::VolumeGridHelperBase > gridHelper;
    base::Node* volumeNode;
    base::Camera* const cam;
    const std::unique_ptr< base::Node > root;
    
    void resetCamTransform();
};


TestScene::Details::Details( TestScene& self, helpers::VolumeGridHelperBase* gridHelper, const base::math::Vector3f& spacing )
    : cam( new base::Camera() )
    , root( new base::Node() )
{
    volumeNode = gridHelper->createNode( GEOMETRY_TYPE_VOLUMETRIC, helpers::VolumeGridHelperBase::Spacing( spacing ) );
    root->attachChild( volumeNode );
    gridHelper->releaseGeometryFeatures();

    /* Configure camera node.
     */
    cam->setProjection( base::math::frustum4f( 3.14f * 45 / 180.f, 1, 10, 2000 ) );
    resetCamTransform();
    root->attachChild( cam );
}


TestScene::Details* TestScene::Details::create( TestScene& self, bool provideNormals )
{
    /* Load test volume data.
     */
    base::math::Vector3f spacing;
    const std::unique_ptr< base::HUVolumeUInt16 > baseVolume
        ( HUGZSceneFactory::importVolume( std::string( SOURCE_PATH ) + "/res/pelves_reduced.hugz", spacing ) );
        
    /* Prepare data.
     */
    helpers::VolumeGridHelperBase* const gridHelper = provideNormals
        ? (helpers::VolumeGridHelperBase*)( new helpers::VolumeGridHelper< base::HUVolumeUInt16, base::NormalMap3DInt8 >( baseVolume->size ) )
        : (helpers::VolumeGridHelperBase*)( new helpers::VolumeGridHelper< base::HUVolumeUInt16, void >( baseVolume->size ) );
    gridHelper->loadData( [&baseVolume]( const base::math::Vector3ui& p )->base::HUV
        {
            return ( *baseVolume )( p );
        }
    );
    
    /* Finish.
     */
    return new Details( self, gridHelper, spacing );
}


void TestScene::Details::resetCamTransform()
{
    cam->localTransform = base::math::translation4f( 0, 0, 350 );
}



// ----------------------------------------------------------------------------------
// TestScene
// ----------------------------------------------------------------------------------

TestScene::TestScene( bool provideNormals )
    : pimpl( Details::create( *this, provideNormals ) )
{
}


TestScene::~TestScene()
{
}


base::Node& TestScene::volumeNode() const
{
    return *pimpl->volumeNode;
}


base::Node& TestScene::root() const
{
    return *pimpl->root;
}


base::Camera& TestScene::cam() const
{
    return *pimpl->cam;
}


void TestScene::resetCamTransform()
{
    pimpl->resetCamTransform();
}



}  // namespace Carna :: testing

}  // namespace Carna
