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

#ifndef FRAMERENDERERFACTORY_H_0874895466
#define FRAMERENDERERFACTORY_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/RenderStageSequence.h>
#include <Carna/base/noncopyable.h>
#include <Carna/base/Aggregation.h>

/** \file   FrameRendererFactory.h
  * \brief  Defines \ref Carna::qt::FrameRendererFactory.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// FrameRendererFactory
// ----------------------------------------------------------------------------------

/** \brief
  * Instantiates `base::FrameRenderer` when a `base::GLContext` is provided.
  *
  * This class supports the same API for managing render stages as
  * `base::FrameRenderer` does. This allows you to use the
  * `helpers::FrameRendererHelper` with this class too.
  *
  * \author Leonid Kostrykin
  * \date   2.4.15
  */
class CARNAQT_LIB FrameRendererFactory : public base::RenderStageSequence
{

    NON_COPYABLE

    std::vector< base::RenderStage* > myStages;

public:
    
    /** \brief
      * Instantiates `base::FrameRenderer` and hands over all rendering stages to it.
      */
    base::FrameRenderer* createRenderer( base::GLContext& glContext, unsigned int width, unsigned int height, bool fitSquare );

}; // FrameRendererFactory



}  // namespace Carna :: qt

}  // namespace Carna

#endif // FRAMERENDERERFACTORY_H_0874895466

