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

#ifndef RENDERSTAGECONTROLDETAILS_H_0874895466
#define RENDERSTAGECONTROLDETAILS_H_0874895466

/** \file   RenderStageControlDetails.h
  * \brief  Defines implementation details of \ref Carna::qt::RenderStageControl.
  */

#include <Carna/qt/RenderStageControl.h>
#include <QObject>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// RenderStageControl :: Details
// ----------------------------------------------------------------------------------

/** \brief
  * Defines \ref RenderStageControl implementation details.
  *
  * \author Leonid Kostrykin
  * \date   2.4.15
  */
class RenderStageControl::Details : public QObject
{

    Q_OBJECT
    NON_COPYABLE

public:

    Details( RenderStageControl& self );

    RenderStageControl& self;
    
    bool updateScheduled;

public slots:

    void updateDisplay();

}; // RenderStageControl :: Details



}  // namespace Carna :: qt

}  // namespace Carna

#endif // RENDERSTAGECONTROLDETAILS_H_0874895466
