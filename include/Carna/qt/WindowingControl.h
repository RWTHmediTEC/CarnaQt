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

class CARNAQT_LIB WindowingControl : public QWidget
{

    Q_OBJECT
    NON_COPYABLE
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    struct Windowing
    {
        virtual ~Windowing();
        virtual void setWindowing( base::HUV windowingLevel, unsigned int windowingWidth ) = 0;
        virtual base::HUV windowingLevel() const = 0;
        virtual unsigned int windowingWidth() const = 0;
    };
    
    template< typename WindowingType >
    struct GenericWindowingAdapter : public Windowing
    {
        GenericWindowingAdapter( WindowingType& windowing );
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
    
    Windowing& windowing();
    
    const Windowing& windowing() const;
    
public slots:

    void setWindowingLevel( int windowingLevel );

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
