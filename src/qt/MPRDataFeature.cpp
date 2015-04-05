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

#include <Carna/qt/MPRDataFeature.h>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPRDataFeature :: ManagedInterface
// ----------------------------------------------------------------------------------

MPRDataFeature::ManagedInterface::ManagedInterface( MPRDataFeature& mprdf )
    : base::GeometryFeature::ManagedInterface( mprdf )
{
}



// ----------------------------------------------------------------------------------
// MPRDataFeature
// ----------------------------------------------------------------------------------

MPRDataFeature::MPRDataFeature()
{
}


bool MPRDataFeature::controlsSameVideoResource( const GeometryFeature& other ) const
{
    return true;
}


MPRDataFeature::ManagedInterface* MPRDataFeature::acquireVideoResource()
{
    return new ManagedInterface( *this );
}



}  // namespace Carna :: qt

}  // namespace Carna

