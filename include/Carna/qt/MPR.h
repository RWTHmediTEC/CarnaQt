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
  * Interconnects multiple \ref MPRDisplay instances and realizes their juncture to
  * the scene.
  *
  * Use the \ref setRoot method to specify the scene to be rendered. The class looks
  * automatically for the volumetric data beneath the root. The current
  * implementation is restricted s.t. it supports one volumetric node/grid per
  * instance. You should simply specify the root node of a subtree if your scene
  * contains multiple volumetric grids. The lifetime of `%MPR` objects is independent
  * from that of the attached root node: The `%MPR` object resets its root reference
  * automatically when the root is destroyed. The lifetime is also independent from
  * that of the attached displays.
  *
  * Each \ref MPRDisplay attaches a dedicated node to the `%MPR` root. This *pivot*
  * node holds the two scene graph leafs that each `%MPRDisplay` requires, namely its
  * `base::Camera` and an `base::Geometry` instance that represents the cutting plane
  * whose normal vector is parallel to the viewing direction. The \ref MPRDisplay
  * also removes the pivot automatically when it is detached from the `%MPR`.
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
  *  -# Now we implement a short program that also uses an \ref WindowingControl
  *     instance:
  *     \snippet Demos/MPRExtended/main.cpp mpr_main
  *
  * \author Leonid Kostrykin
  * \date   5.4.15
  */
class CARNAQT_LIB MPR
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      *
      * \param geometryTypeVolume is the geometry type to be used to identify
      *     volumetric data.
      */
    explicit MPR( unsigned int geometryTypeVolume );
    
    /** \brief
      * Detaches all displays.
      */
    ~MPR();
    
    /** \brief
      * Identifies volumetric data.
      */
    const unsigned int geometryTypeVolume;
    
    /** \brief
      * Sets \a root as the root node of the scene. Triggers a search for data whose
      * geometry type matches \ref geometryTypeVolume.
      *
      * Use the \ref hasVolume and \ref volume methods to query the results of this
      * search.
      *
      * \throws base::AssertionFailure if more than one volumetric grid is found.
      */
    void setRoot( base::Node& root );
    
    /** \brief
      * Adds \a mprDisplay to this `%MPR`. This is equivalent to
      * \ref MPRDisplay::setMPR.
      *
      * If the \a mprDisplay is attached to a different `%MPR`, it is
      * \ref removeDisplay "removed" from that first.
      */
    void addDisplay( MPRDisplay& mprDisplay );
    
    /** \brief
      * Removes \a mprDisplay from this `%MPR`. This is equivalent to
      * \ref MPRDisplay::removeFromMPR.
      *
      * Nothing happens if \a mprDisplay is not attached to this `%MPR`.
      */
    void removeDisplay( MPRDisplay& mprDisplay );
    
    /** \brief
      * Tells whether a volumetric grid has been found beneath the
      * \ref setRoot "root".
      */
    bool hasVolume() const;
    
    /** \brief
      * References the volumetric grid found beneath the \ref setRoot "root".
      * \pre `hasVolume() == true`
      */
    base::Spatial& volume();
    
    /** \overload
      */
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

