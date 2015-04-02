#pragma once

#include <HUIO.h>
#include <Carna/base/math.h>
#include <Carna/base/BufferedHUVolume.h>
#include <fstream>
#include <QDebug>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace Carna
{

namespace testing
{



// ----------------------------------------------------------------------------------
// HUGZSceneFactory
// ----------------------------------------------------------------------------------

/** \brief
  * Creates \ref Carna::base::HUVolumeUInt16 object from HUGZ-file.
  *
  * \section HUGZFileFormat HUGZ File Format
  *
  * The HUGZ file is GZIP compression of the following data:
  * -# Bytes 1 to 4 are an unsigned integer that describes the volume width.
  * -# Bytes 5 to 8 are an unsigned integer that describes the volume height.
  * -# Bytes 9 to 12 are an unsigned integer that describes the volume depth.
  * -# Bytes 13 to 16 are an IEEE 754 single precision floating point number that describes the x-spacing.
  * -# Bytes 17 to 20 are an IEEE 754 single precision floating point number that describes the y-spacing.
  * -# Bytes 21 to 24 are an IEEE 754 single precision floating point number that describes the z-spacing.
  * -# Each voxel is represented as a \c signed \c short.
  *
  * \todo
  * Use `int16_t` instead of `signed short`.
  */
struct HUGZSceneFactory
{
    /** \brief
      * Reads HUGZ file and returns created \ref Carna::base::HUVolumeUInt16 object.
      *
      * The HUGZ file format is described \ref HUGZFileFormat "here".
      */
    static Carna::base::HUVolumeUInt16* importVolume( const std::string& filename, Carna::base::math::Vector3f& spacing );
};



}  // namespace testing

}  // namespace Carna
