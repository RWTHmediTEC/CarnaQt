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

#ifndef WINDOWINGCONTROL_H_0874895466
#define WINDOWINGCONTROL_H_0874895466

/** \file   WindowingControl.h
  * \brief  Defines \ref Carna::qt::WindowingControl
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/qt/RenderStageControl.h>
#include <Carna/presets/CuttingPlanesStage.h>
#include <Carna/base/noncopyable.h>
#include <QWidget>
#include <memory>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// WindowingControl
// ----------------------------------------------------------------------------------

/** \brief
  * Defines widget that controls HU windowing. This is the widget illustrated and
  * used in the \ref MPR examples.
  *
  * Objects that `%WindowingControl` control must implement the \ref Windowing
  * interface. The predefined \ref GenericWindowingAdapter can be used to achieve
  * this through compile-time polymorphism, as the
  * \ref MPRExtendedExample "extended MPR example" shows.
  *
  * \section WindowingControlExample Example
  *
  * This example shows the `%qt::WindowingControl` widget using an
  * `presets::CuttingPlanesStage` instance. The
  * \ref MPRExtendedExample "extended MPR example" shows its usage for multiplanar
  * reformats.
  *
  *  -# Start with including headers from base %Carna and this package:
  *     \snippet Demos/WindowingControl/main.cpp windowing_includes
  *  -# The cutting-planes rendering stage requires us to define two geometry types
  *     at least:
  *     \snippet Demos/WindowingControl/main.cpp windowing_constants
  *     For explanation on the concept of geometry types, refer to the base %Carna
  *     documentation.
  *  -# Now we implement a short program:
  *     \snippet Demos/WindowingControl/main.cpp windowing_main
  *
  * \author Leonid Kostrykin
  * \date   6.4.15
  */
class CARNAQT_LIB WindowingControl : public QWidget
{

    Q_OBJECT
    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Defines the interface that \ref WindowingControl controls.
      */
    struct Windowing
    {
        /** \brief
          * Does nothing.
          */
        virtual ~Windowing();
        
        /** \brief
          * Updates windowing.
          */
        virtual void setWindowing( base::HUV windowingLevel, unsigned int windowingWidth ) = 0;
        
        /** \brief
          * Tells current windowing level.
          */
        virtual base::HUV windowingLevel() const = 0;
        
        /** \brief
          * Tells current windowing width.
          */
        virtual unsigned int windowingWidth() const = 0;
    };
    
    /** \brief
      * Implements the \ref Windowing interface as an adapter to \a WindowingType in
      * terms of compile-time polymorphism.
      *
      * The unspecialized version expects \a WindowingType to support the following
      * methods in UML notation:
      *
      *   - `setWindowingLevel(level : base::HUV)`
      *   - `setWindowingWidth(width : unsigned int)`
      *   - `%windowingLevel() : base::HUV`
      *   - `%windowingWidth() : unsigned int`
      *
      * Specialize the template to overcome this expectations.
      */
    template< typename WindowingType >
    struct GenericWindowingAdapter : public Windowing
    {
        /** \brief
          * Instanciates object that will delegate the \ref Windowing interface to
          * \a windowing. 
          */
        GenericWindowingAdapter( WindowingType& windowing );
        
        /** \brief
          * The object that the \ref Windowing interface is delegated to.
          */
        WindowingType& windowing;
        
        virtual void setWindowing( base::HUV windowingLevel, unsigned int windowingWidth ) override;
        virtual base::HUV windowingLevel() const override;
        virtual unsigned int windowingWidth() const override;
        
    private:
        
        const std::unique_ptr< RenderStageControl > rsControl;
    };

    /** \brief
      * Instantiates. Does not take possession of \a windowing.
      */
    explicit WindowingControl( Windowing& windowing, QWidget* parent = nullptr );
    
    /** \brief
      * Instantiates and takes possession of \a windowing.
      */
    explicit WindowingControl( Windowing* windowing, QWidget* parent = nullptr );

    /** \brief
      * Deletes.
      */
    virtual ~WindowingControl();
    
    /** \brief
      * References the controlled object.
      */
    Windowing& windowing();
    
    /** \overload
      */
    const Windowing& windowing() const;
    
public slots:

    /** \brief
      * Updates UI and invokes \ref Windowing::setWindowing on \ref windowing.
      */
    void setWindowingLevel( int windowingLevel );

    /** \brief
      * Updates UI and invokes \ref Windowing::setWindowing on \ref windowing.
      */
    void setWindowingWidth( int windowingWidth );

}; // WindowingControl



// ----------------------------------------------------------------------------------
// WindowingControl :: GenericWindowingAdapter< WindowingType >
// ----------------------------------------------------------------------------------

template< typename WindowingType >
WindowingControl::GenericWindowingAdapter< WindowingType >::GenericWindowingAdapter( WindowingType& windowing )
    : windowing( windowing )
{
}


template< typename WindowingType >
void WindowingControl::GenericWindowingAdapter< WindowingType >::setWindowing( base::HUV windowingLevel, unsigned int windowingWidth )
{
    windowing.setWindowingLevel( windowingLevel );
    windowing.setWindowingWidth( windowingWidth );
}


template< typename WindowingType >
base::HUV WindowingControl::GenericWindowingAdapter< WindowingType >::windowingLevel() const
{
    return windowing.windowingLevel();
}


template< typename WindowingType >
unsigned int WindowingControl::GenericWindowingAdapter< WindowingType >::windowingWidth() const
{
    return windowing.windowingWidth();
}



// ----------------------------------------------------------------------------------
// WindowingControl :: GenericWindowingAdapter< presets::CuttingPlanesStage >
// ----------------------------------------------------------------------------------

template< >
WindowingControl::GenericWindowingAdapter< presets::CuttingPlanesStage >::GenericWindowingAdapter( presets::CuttingPlanesStage& stage )
    : windowing( stage )
    , rsControl( new RenderStageControl( stage ) )
{
}


template< >
void WindowingControl::GenericWindowingAdapter< presets::CuttingPlanesStage >::setWindowing
    ( base::HUV windowingLevel
    , unsigned int windowingWidth )
{
    windowing.setWindowingLevel( windowingLevel );
    windowing.setWindowingWidth( windowingWidth );
    rsControl->invalidate();
}




}  // namespace Carna :: qt

}  // namespace Carna

#endif // WINDOWINGCONTROL_H_0874895466
