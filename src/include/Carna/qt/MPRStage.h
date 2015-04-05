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

#ifndef MPRSTAGE_H_0874895466
#define MPRSTAGE_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/GeometryStage.h>
#include <memory>

/** \file   MPRStage.h
  * \brief  Defines \ref Carna::qt::MPRStage.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPRStage
// ----------------------------------------------------------------------------------

class MPRStage : public base::GeometryStage< void >
{

    NON_COPYABLE
    
    struct Details;
    std::unique_ptr< Details > pimpl;

public:

    const static unsigned int ROLE_PLANE_DATA = 0;

    explicit MPRStage( unsigned int geometryTypePlanes );
        
    virtual ~MPRStage();

    virtual void renderPass
        ( const base::math::Matrix4f& viewTransform
        , base::RenderTask& rt
        , const base::Viewport& vp ) override;
    
protected:

    virtual void render( const base::Renderable& ) override;

}; // MPRStage



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MPRSTAGE_H_0874895466

