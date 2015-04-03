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

#ifndef CARNAQT_GLOBAL_H_0874895466
#define CARNAQT_GLOBAL_H_0874895466

#if defined( CARNAQT_EXPORT )
#   if defined( _MSC_VER )
#       define CARNAQT_LIB __declspec( dllexport )
#       pragma warning( disable : 4251 )
#   elif defined( _GCC )
#       define CARNAQT_LIB __attribute__( ( visibility( "default" ) ) )
#   else
#       define CARNAQT_LIB
#   endif
#else
#   if defined( _MSC_VER )
#       define CARNAQT_LIB __declspec( dllimport )
#   else
#       define CARNAQT_LIB
#   endif
#endif
#if defined( NO_OVERRIDE_KEYWORD )
#   define override
#endif

#include <Carna/Carna.h>



// ----------------------------------------------------------------------------------
// Forward Declarations
// ----------------------------------------------------------------------------------

namespace Carna
{

    /** \brief
      * Contains the Qt framework components.
      */
    namespace qt
    {
        class Application;
        class ColorMapEditor;
        class ColorMapSpanPainter;
        class ColorMapTracker;
        class ColorMapTrackerEditor;
        class ColorPicker;
        class ColorPickerPainter;
        class Display;
        class DRRControl;
        class DVRControl;
        class ExpandableGroupBox;
        class FrameRendererFactory;
        class IntSpanPainter;
        class MIPControlLayer;
        class MIPLayerEditor;
        class MultiSpanSlider;
        class MultiSpanSliderModelViewMapping;
        class MultiSpanSliderTracker;
        class NullIntSpanPainter;
        class RenderStageControl;
        class VolumeRenderingControl;
        class WideColorPicker;
    }

}  // namespace Carna

#endif // CARNAQT_GLOBAL_H_0874895466

