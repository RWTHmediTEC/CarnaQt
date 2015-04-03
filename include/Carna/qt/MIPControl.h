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

#ifndef MIPCONTROL_H_0874895466
#define MIPCONTROL_H_0874895466

/** \file   MIPControl.h
  * \brief  Defines \ref Carna::qt::MIPControl.
  */

#include <Carna/qt/CarnaQt.h>
#include <Carna/qt/VolumeRenderingControl.h>
#include <memory>

class QPushButton;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MIPControl
// ----------------------------------------------------------------------------------

/** \brief
  * Defines `presets::%MIPStage` controlling widget.
  *
  * \image html MIPControl.png
  *
  * \section MIPControlExample Example
  *
  *  -# Start with including headers from base %Carna and this packge:
  *     \snippet Demos/MIPControl/main.cpp mip_includes
  *  -# Lets stick with a single geometry type for simplicity:
  *     \snippet Demos/MIPControl/main.cpp mip_constants
  *     For explanation on the concept of geometry types, refer to the base %Carna
  *     documentation.
  *  -# Now we implement a short program:
  *     \snippet Demos/MIPControl/main.cpp mip_main
  *
  * \author Leonid Kostrykin
  * \date   16.4.12 - 3.4.15
  */
class CARNAQT_LIB MIPControl : public VolumeRenderingControl
{

    Q_OBJECT
    
    struct Details;
    const std::unique_ptr< Details > pimpl;

public:

    /** \brief
      * Instantiates.
      */
    explicit MIPControl( presets::MIPStage& mip, QWidget* parent = nullptr );
    
    /** \brief
      * Deletes.
      */
    virtual ~MIPControl();
    
    presets::MIPStage& mip;
    
    void appendLayer( presets::MIPLayer* layer );
    
public slots:

    void appendLayer();
    
    void clearLayers();
    
    void saveColorConfig();
    
    void loadColorConfig();

}; // MIPControl



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MIPCONTROL_H_0874895466
