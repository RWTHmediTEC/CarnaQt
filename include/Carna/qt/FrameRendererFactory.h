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
class CARNAQT_LIB FrameRendererFactory
{

    NON_COPYABLE

    std::vector< base::RenderStage* > myStages;

public:

    /** \copybrief reset
      */
    ~FrameRendererFactory();
    
    /** \brief
      * Deletes stages that haven't been transfered to an instantiated
      * `base::FrameRenderer` yet.
      *
      * This method is provided for convenience. It does the same as
      * \ref clearStages does.
      */
    void reset();
    
    /** \brief
      * Instantiates `base::FrameRenderer` and hands over all rendering stages to it.
      */
    base::FrameRenderer* createRenderer( base::GLContext& glContext, unsigned int width, unsigned int height, bool fitSquare );

    /** \brief
      * Tells number of stages contained by this renderer.
      */
    std::size_t stages() const;
    
    /** \brief
      * Appends \a stage to the rendering stages sequence.
      */
    void appendStage( base::RenderStage* stage );
    
    /** \brief
      * Deletes all stages from the rendering stages sequence.
      */
    void clearStages();
    
    /** \brief
      * References the stage at \a position withing the rendering stages sequence.
      */
    base::RenderStage& stageAt( std::size_t position ) const;

    /** \brief
      * References the first \a RenderStage within the rendering stages sequence.
      * This method performs a linear search. Returns \ref Aggregation::NULL_PTR if
      * \a RenderStage is not found withing hte rendering stages sequence.
      */
    template< typename RenderStage >
    base::Aggregation< RenderStage > findStage() const;

}; // FrameRendererFactory


template< typename RenderStageType >
base::Aggregation< RenderStageType > FrameRendererFactory::findStage() const
{
    for( std::size_t index = 0; index < stages(); ++index )
    {
        RenderStageType* const rs = dynamic_cast< RenderStageType* >( &stageAt( index ) );
        if( rs != nullptr )
        {
            return base::Aggregation< RenderStageType >( *rs );
        }
    }
    return base::Aggregation< RenderStageType >::NULL_PTR;
}



}  // namespace Carna :: qt

}  // namespace Carna

#endif // FRAMERENDERERFACTORY_H_0874895466

