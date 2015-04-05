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

#ifndef MPRDATAFEATURE_H_0874895466
#define MPRDATAFEATURE_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/GeometryFeature.h>
#include <Carna/base/Color.h>

/** \file   MPRDataFeature.h
  * \brief  Defines \ref Carna::qt::MPRDataFeature.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPRDataFeature
// ----------------------------------------------------------------------------------

class MPRDataFeature : public base::GeometryFeature
{

    NON_COPYABLE

public:

    MPRDataFeature();
    
    base::Color color;
    
    virtual bool controlsSameVideoResource( const GeometryFeature& other ) const override;
    
    // ------------------------------------------------------------------------------
    // MPRDataFeature :: ManagedInterface
    // ------------------------------------------------------------------------------
    
    class ManagedInterface : public base::GeometryFeature::ManagedInterface
    {
    
    public:
    
        ManagedInterface( MPRDataFeature& );
    
    };
    
    virtual ManagedInterface* acquireVideoResource() override;

}; // MPRDataFeature



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MPRDATAFEATURE_H_0874895466

