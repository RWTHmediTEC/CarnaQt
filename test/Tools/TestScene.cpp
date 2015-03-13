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
#include <Carna/base/Node.h>
#include <Carna/base/math.h>
#include <Carna/base/Camera.h>
#include <Carna/base/Geometry.h>
#include <Carna/base/BufferedHUVolumeTexture.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestScene
// ----------------------------------------------------------------------------------

TestScene::TestScene( base::GLContext& glContext )
    : myVolumeGeometry( new base::Geometry( GEOMETRY_TYPE_VOLUMETRIC ) )
    , myCam( new base::Camera() )
    , glContext( glContext )
    , root( new base::Node() )
{
    /* Load test volume data.
     */
    base::math::Vector3f spacing;
    huVolume.reset( HUGZSceneFactory::importVolume( SOURCE_PATH + "/res/pelves_reduced.hugz", spacing ) );
    const base::math::Vector3f scale
        ( ( huVolume->size.x() - 1 ) * spacing.x()
        , ( huVolume->size.y() - 1 ) * spacing.y()
        , ( huVolume->size.z() - 1 ) * spacing.z() );

    /* Upload to GPU.
     */
    glContext.makeActive();
    base::BufferedHUVolumeTexture< base::UInt16HUVolume >& volumeTexture
        = base::BufferedHUVolumeTexture< base::UInt16HUVolume >::create( *huVolume );

    /* Configure geometry node for volume data.
     */
    myVolumeGeometry->putFeature( ROLE_HU_VOLUME, volumeTexture );
    myVolumeGeometry->localTransform = base::math::scaling4f( scale );
    root->attachChild( myVolumeGeometry );
    volumeTexture.release();

    /* Configure camera node.
     */
    myCam->setProjection( base::math::frustum4f( 3.14f * 45 / 180.f, 1, 10, 2000 ) );
    resetCamTransform();
    root->attachChild( myCam );
}


TestScene::~TestScene()
{
}


base::Geometry& TestScene::volumeGeometry() const
{
    return *myVolumeGeometry;
}


base::Camera& TestScene::cam() const
{
    return *myCam;
}


void TestScene::resetCamTransform()
{
    myCam->localTransform = base::math::translation4f( 0, 0, 350 );
}



}  // namespace Carna :: testing

}  // namespace Carna
