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
        
    virtual ~MPR();
    
    const unsigned int geometryTypeVolume;
    
    void setRoot( base::Node& root );
    
    void addDisplay( MPRDisplay& mprDisplay );
    
    void removeDisplay( MPRDisplay& mprDisplay );

}; // MPR



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MPR_H_0874895466

