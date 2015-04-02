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

#ifndef APPLICATION_H_0874895466
#define APPLICATION_H_0874895466

#include <Carna/qt/CarnaQt.h>
#include <Carna/base/noncopyable.h>
#include <QApplication>

/** \file   Application.h
  * \brief  Defines \ref Carna::qt::Application.
  */

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// Application
// ----------------------------------------------------------------------------------

/** \brief
  * Specializes `QApplication` s.t. %Carna exceptions are processed properly. Also
  * setups the `base::Log` s.t. it gets Qt-compatible.
  *
  * \author Leonid Kostrykin
  * \date   2.4.15
  */
class CARNAQT_LIB Application : public QApplication
{

    NON_COPYABLE

public:

    Application( int& argc, char** argv );
    virtual ~Application();
    
    virtual bool notify( QObject* receiver, QEvent* ev ) override;

}; // Application



}  // namespace Carna :: qt

}  // namespace Carna

#endif // APPLICATION_H_0874895466

