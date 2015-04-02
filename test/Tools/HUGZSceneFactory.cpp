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

#include "HUGZSceneFactory.h"

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// stream_read
// ----------------------------------------------------------------------------------

template< typename StreamType, typename ValueType >
void stream_read( StreamType& in, ValueType& out )
{
    in.read( reinterpret_cast< char* >( &out ), sizeof( out ) );
}



// ----------------------------------------------------------------------------------
// HUGZSceneFactory
// ----------------------------------------------------------------------------------

Carna::base::HUVolumeUInt16* HUGZSceneFactory::importVolume( const std::string& filename, Carna::base::math::Vector3f& spacing )
{
    std::ifstream file( filename, std::ios::in | std::ios::binary );
    CARNA_ASSERT( file.is_open() && !file.fail() );
    boost::iostreams::filtering_istream in;
    in.push( boost::iostreams::gzip_decompressor() );
    in.push( file );

    Carna::base::math::Vector3ui size;
    stream_read( in, size.x() );
    stream_read( in, size.y() );
    stream_read( in, size.z() );

    Carna::base::HUVolumeUInt16* const volume = new Carna::base::HUVolumeUInt16( size );

    stream_read( in, spacing.x() );
    stream_read( in, spacing.y() );
    stream_read( in, spacing.z() );

    HUIO::Reader reader( in );
    for( unsigned int z = 0; z < size.z(); ++z )
    for( unsigned int y = 0; y < size.y(); ++y )
    for( unsigned int x = 0; x < size.x(); ++x )
    {
        const signed short huv = reader.read();
        volume->setVoxel( x, y, z, huv );
    }

    return volume;
}



}  // namespace Carna :: testing

}  // namespace Carna
