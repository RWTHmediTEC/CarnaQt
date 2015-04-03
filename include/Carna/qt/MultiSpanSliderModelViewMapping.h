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

#ifndef MULTISPANSLIDERMODELVIEWMAPPING_H_0874895466
#define MULTISPANSLIDERMODELVIEWMAPPING_H_0874895466

/** \file   MultiSpanSliderModelViewMapping.h
  * \brief  Defines \ref Carna::qt::MultiSpanSliderModelViewMapping.
  */

#include <Carna/qt/CarnaQt.h>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MultiSpanSliderModelViewMapping
// ----------------------------------------------------------------------------------

/** \brief
  * Defines the mapping of numerical values from model space to pixels in view space
  * and vise versa for \ref MultiSpanSlider objects.
  *
  * \author Leonid Kostrykin
  * \date   12.8.11 - 2.4.15
  */
class CARNAQT_LIB MultiSpanSliderModelViewMapping
{

public:

    /** \brief
      * Instantiates.
      */
    MultiSpanSliderModelViewMapping( int first, int last, int view_height, unsigned int view_offset_top, unsigned int view_offset_left )
        : vals_per_pixel( static_cast< float >( last - first ) / view_height )
        , pixels_per_val( 1 / vals_per_pixel )
        , bias( first * pixels_per_val )
        , view_offset_top( view_offset_top )
        , view_offset_left( view_offset_left )
    {
    }

    /** \brief
      * Instantiates.
      */
    MultiSpanSliderModelViewMapping()
        : view_offset_top( 0 )
        , view_offset_left( 0 )
        , vals_per_pixel( 1 )
        , pixels_per_val( 1 )
    {
    }

    /** \brief
      * Returns the pixel value corresponding to the given model space value.
      */
    int valueToPixel( int value ) const
    {
        return static_cast< int >( view_offset_top ) - bias + pixels_per_val * value;
    }

    /** \brief
      * Returns the model space value corresponding to the given pixel value.
      */
    int pixelToValue( int pixel ) const
    {
        return vals_per_pixel * ( pixel - static_cast< int >( view_offset_top ) + bias );
    }

    /** \brief
      * Returns the \f$x\f$ offset, assuming that the \ref MultiSpanSlider is
      * oriented vertically.
      */
    unsigned int getOffsetLeft() const
    {
        return view_offset_left;
    }

    /** \brief
      * Returns the \f$y\f$ offset, assuming that the \ref MultiSpanSlider is
      * oriented vertically.
      */
    unsigned int getOffsetTop() const
    {
        return view_offset_top;
    }

private:

    /** \brief
      * Holds the number of model space values mapped per pixel.
      */
    float vals_per_pixel;

    /** \brief
      * Holds the number of pixels occupied by a single model space value.
      */
    float pixels_per_val;

    /** \brief
      * Holds the model space bias.
      */
    int bias;

    /** \brief
      * Holds the \f$y\f$ offset, assuming that the \ref MultiSpanSlider is oriented
      * vertically.
      */
    unsigned int view_offset_top;

    /** \brief
      * Holds the \f$x\f$ offset, assuming that the \ref MultiSpanSlider is oriented
      * vertically.
      */
    unsigned int view_offset_left;

}; // MultiSpanSliderModelViewMapping



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MULTISPANSLIDERMODELVIEWMAPPING_H_0874895466
