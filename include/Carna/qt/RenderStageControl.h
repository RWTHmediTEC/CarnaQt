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

#ifndef RENDERSTAGECONTROL_H_0874895466
#define RENDERSTAGECONTROL_H_0874895466

/** \file   RenderStageControl.h
  * \brief  Defines \ref Carna::qt::RenderStageControl.
  */

#include <Carna/qt/CarnaQt.h>
#include <memory>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// RenderStageControl
// ----------------------------------------------------------------------------------

/** \brief
  * Defines base class of `base::%RenderStage` controllers.
  *
  * \author Leonid Kostrykin
  * \date   2.4.15
  */
class CARNAQT_LIB RenderStageControl
{

    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

protected:

    explicit RenderStageControl( base::RenderStage& rs );
    
    /** \brief
      * Deletes.
      */
    ~RenderStageControl();
    
    base::RenderStage& renderStage;
    
    void invalidate();
    
    virtual void onRenderingStarted();
    
    virtual void onRenderingFinished();

}; // RenderStageControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // RENDERSTAGECONTROL_H_0874895466
