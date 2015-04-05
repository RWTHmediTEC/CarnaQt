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

#include <Carna/base/text.h>
#include <Carna/base/Log.h>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPRDataFeature
// ----------------------------------------------------------------------------------

MPRDataFeature::MPRDataFeature()
{
}


bool MPRDataFeature::controlsSameVideoResource( const GeometryFeature& other ) const
{
    return false;
}


MPRDataFeature::ManagedInterface* MPRDataFeature::acquireVideoResource()
{
    return nullptr;
}



}  // namespace Carna :: qt

}  // namespace Carna

