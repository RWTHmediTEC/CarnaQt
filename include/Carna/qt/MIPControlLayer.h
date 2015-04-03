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

#ifndef MIPCONTROLLAYER_H_0874895466
#define MIPCONTROLLAYER_H_0874895466

/** \file   MIPControlLayer.h
  * \brief  Defines \ref Carna::qt::MIPControlLayer.
  */

#include <Carna/qt/CarnaQt.h>
#include <QWidget>
#include <functional>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MIPControlLayer
// ----------------------------------------------------------------------------------

/** \brief
  * Represents single `presets::MIPLayer` object within an `MIPControl` object.
  *
  * This class extends the \ref MIPLayerEditor with buttons for deleting, ascending
  * and descending the layer by priority.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11 - 3.4.15
  */
class CARNAQT_LIB MIPControlLayer : public QWidget
{

    Q_OBJECT

public:

    /** \brief
      * Instantiates.
      */
    explicit MIPControlLayer( presets::MIPStage& stage, presets::MIPLayer& layer, QWidget* parent = nullptr );

    /** \brief
      * Returns the optimal size.
      */
    virtual QSize sizeHint() const;
    
    /** \brief
      * References the `presets::MIPStage` the edited layer belongs to.
      */
    presets::MIPStage& stage;

    /** \brief
      * References the underlying `presets::MIPLayer` editor.
      */
    MIPLayerEditor& editor;

public slots:

    void remove();

    void ascend();

    void descend();
    
signals:

    void changed();
    
private slots:

    void invalidate();

}; // MIPControlLayer



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MIPCONTROLLAYER_H_0874895466
