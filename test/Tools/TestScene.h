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

#pragma once

#include <Carna/Carna.h>
#include <Carna/base/UInt16HUVolume.h>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestScene
// ----------------------------------------------------------------------------------

class TestScene
{

    std::unique_ptr< base::UInt16HUVolume > huVolume;

    base::Geometry* const myVolumeGeometry;

    base::Camera* const myCam;

public:

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;

    const static unsigned int ROLE_HU_VOLUME = 0;

    base::GLContext& glContext;

    TestScene( base::GLContext& glContext );

    ~TestScene();

    const std::unique_ptr< base::Node > root;

    base::Geometry& volumeGeometry() const;

    base::Camera& cam() const;

    void resetCamTransform();

}; // TestScene



}  // namespace testing

}  // namespace Carna
