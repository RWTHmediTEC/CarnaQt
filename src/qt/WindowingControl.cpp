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

#include <Carna/qt/WindowingControl.h>
#include <Carna/base/Composition.h>
#include <Carna/base/Aggregation.h>
#include <QSlider>
#include <QFormLayout>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// WindowingControl :: Details
// ----------------------------------------------------------------------------------

struct WindowingControl::Details
{
    Details( base::Association< Windowing >* windowing );
    const std::unique_ptr< base::Association< Windowing > > windowing;
    
    QSlider* const slLevel;
    QSlider* const slWidth;
    
    void createUI( WindowingControl& self );
};


WindowingControl::Details::Details( base::Association< WindowingControl::Windowing >* windowing )
    : windowing( windowing )
    , slLevel( new QSlider( Qt::Horizontal ) )
    , slWidth( new QSlider( Qt::Horizontal ) )
{
}


void WindowingControl::Details::createUI( WindowingControl& self )
{
    slLevel->setMinimum( -1024 );
    slLevel->setMaximum( +3071 );
    slLevel->setValue  ( self.windowing().windowingLevel() );
    
    slWidth->setMinimum(  256 );
    slWidth->setMaximum( 4096 );
    slWidth->setValue  ( self.windowing().windowingWidth() );
    
    QObject::connect( slLevel, SIGNAL( valueChanged( int ) ), &self, SLOT( setWindowingLevel( int ) ) );
    QObject::connect( slWidth, SIGNAL( valueChanged( int ) ), &self, SLOT( setWindowingWidth( int ) ) );

    QFormLayout* const form = new QFormLayout();
    form->addRow( "Windowing Level:", slLevel );
    form->addRow( "Windowing Width:", slWidth );
    self.setLayout( form );
}



// ----------------------------------------------------------------------------------
// WindowingControl :: Windowing
// ----------------------------------------------------------------------------------

WindowingControl::Windowing::~Windowing()
{
}



// ----------------------------------------------------------------------------------
// WindowingControl
// ----------------------------------------------------------------------------------

WindowingControl::WindowingControl( Windowing& windowing, QWidget* parent )
    : QWidget( parent )
    , pimpl( new Details( new base::Aggregation< Windowing >( windowing ) ) )
{
    pimpl->createUI( *this );
}


WindowingControl::WindowingControl( Windowing* windowing, QWidget* parent )
    : QWidget( parent )
    , pimpl( new Details( new base::Composition< Windowing >( windowing ) ) )
{
    pimpl->createUI( *this );
}


WindowingControl::~WindowingControl()
{
}


WindowingControl::Windowing& WindowingControl::windowing()
{
    return **pimpl->windowing;
}


const WindowingControl::Windowing& WindowingControl::windowing() const
{
    return **pimpl->windowing;
}


void WindowingControl::setWindowingLevel( int windowingLevel )
{
    windowing().setWindowing( windowingLevel, windowing().windowingWidth() );
}


void WindowingControl::setWindowingWidth( int windowingWidth )
{
    windowing().setWindowing( windowing().windowingLevel(), windowingWidth );
}



}  // namespace Carna :: qt

}  // namespace Carna
