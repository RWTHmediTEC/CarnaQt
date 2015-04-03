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

#ifndef MIPCONTROLDETAILS_H_0874895466
#define MIPCONTROLDETAILS_H_0874895466

/** \file   MIPControlDetails.h
  * \brief  Defines implementation details of \ref Carna::qt::MIPControl.
  */

#include <Carna/qt/MIPControl.h>
#include <Carna/base/RotatingColor.h>

class QVBoxLayout;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MIPControl :: Details
// ----------------------------------------------------------------------------------

class MIPControl :: Details : public QObject
{

    Q_OBJECT

public:

    explicit Details( MIPControl& self );
    MIPControl& self;
    
    QVBoxLayout* const layers;
    base::RotatingColor nextColor;
    
    std::vector< MIPControlLayer* > controls;
    void appendControl( presets::MIPLayer& layer );
    
private slots:

    void invalidate();
    
    void ascend( MIPControlLayer& );
    
    void descend( MIPControlLayer& );
    
    void remove( MIPControlLayer& );
    
private:

    void updateLayersView();

}; // MIPControl :: Details



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MIPCONTROLDETAILS_H_0874895466
