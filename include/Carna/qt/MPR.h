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

#ifndef MPR_H_0874895466
#define MPR_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <memory>

/** \file   MPR.h
  * \brief  Defines \ref Carna::qt::MPR.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MPR
// ----------------------------------------------------------------------------------

/** \brief
  * Interconnects multiple \ref MPRDisplay instances.
  *
  * \image html MPR.png "examplary screenshot from the extended example below"
  *
  * \section MPRMinimalExample Minimal Example
  *
  *  -# Start with including headers from base %Carna and this package:
  *     \snippet Demos/MPRMinimal/main.cpp mpr_includes
  *  -# We must define two geometry types at least:
  *     \snippet Demos/MPRMinimal/main.cpp mpr_constants
  *     For explanation on the concept of geometry types, refer to the base %Carna
  *     documentation.
  *  -# Now we implement a short program:
  *     \snippet Demos/MPRMinimal/main.cpp mpr_main
  *
  * \section MPRExtendedExample Extended Example
  *
  *  -# Again, start with including headers from base %Carna and this package:
  *     \snippet Demos/MPRExtended/main.cpp mpr_includes
  *  -# Lets define an extra geometry type for opaque mesh-based geomtries:
  *     \snippet Demos/MPRExtended/main.cpp mpr_constants
  *  -# Lets also use some extra rendering stages to achieve renderings as shown in
  *     the screenshot above:
  *     \snippet Demos/MPRExtended/main.cpp mpr_extra_stages
  *  -# Now we implement a short program that also uses a \ref WindowingControl:
  *     \snippet Demos/MPRExtended/main.cpp mpr_main
  *
  * \author Leonid Kostrykin
  * \date   5.4.15
  */
class CARNAQT_LIB MPR
{

    NON_COPYABLE
    
    struct Details;
    std::unique_ptr< Details > pimpl;

public:

    explicit MPR( unsigned int geometryTypeVolume );
        
    ~MPR();
    
    const unsigned int geometryTypeVolume;
    
    void setRoot( base::Node& root );
    
    void addDisplay( MPRDisplay& mprDisplay );
    
    void removeDisplay( MPRDisplay& mprDisplay );
    
    bool hasVolume() const;
    
    base::Spatial& volume();
    
    const base::Spatial& volume() const;

    /** \brief
      * Sets windowing level to \a windowingLevel on all attached displays.
      */
    void setWindowingLevel( base::HUV windowingLevel );
    
    /** \brief
      * Sets windowing level to \a windowingWidth on all attached displays.
      */
    void setWindowingWidth( unsigned int windowingWidth );

    /** \brief
      * Tells the windowing level.
      */
    base::HUV windowingLevel() const;
    
    /** \brief
      * Tells the windowing width.
      */
    unsigned int windowingWidth() const;

}; // MPR



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MPR_H_0874895466

