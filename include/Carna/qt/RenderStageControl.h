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
  * Basic `base::%RenderStage` control class.
  *
  * Defines \ref invalidate "invalidation routine" that updates the corresponding
  * `%Display` of the controlled rendering stage. Also provides callbacks for when
  * the rendering, that was triggered by that method, starts and ends.
  *
  * \author Leonid Kostrykin
  * \date   2.4.15
  */
class CARNAQT_LIB RenderStageControl
{

    NON_COPYABLE
    
    class Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    explicit RenderStageControl( base::RenderStage& rs );
    
    /** \brief
      * Deletes.
      */
    ~RenderStageControl();
    
    /** \brief
      * References the controlled rendering stage.
      */
    base::RenderStage& renderStage;
    
    /** \brief
      * Denotes that the corresponding display should update its rendering.
      *
      * An appropriate event is posted to the Qt message queue.
      */
    void invalidate();

protected:
    
    /** \brief
      * Invoked right before rendering starts that was triggered by \ref invalidate.
      */
    virtual void onRenderingStarted();
    
    /** \brief
      * Invoked right after rendering ends that was triggered by \ref invalidate.
      */
    virtual void onRenderingFinished();

}; // RenderStageControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // RENDERSTAGECONTROL_H_0874895466
