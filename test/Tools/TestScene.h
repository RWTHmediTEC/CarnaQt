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
#include <memory>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// TestScene
// ----------------------------------------------------------------------------------

class TestScene
{

    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    const static unsigned int GEOMETRY_TYPE_VOLUMETRIC = 0;

    const static unsigned int ROLE_HU_VOLUME = 0;
    
    const static bool NORMAL_MAP_REQUIRED     = true;
    const static bool NORMAL_MAP_NOT_REQUIRED = false;

    explicit TestScene( bool provideNormals );

    ~TestScene();

    base::Node& volumeNode() const;
    
    base::Node& root() const;

    base::Camera& cam() const;

    void resetCamTransform();

}; // TestScene



}  // namespace testing

}  // namespace Carna
