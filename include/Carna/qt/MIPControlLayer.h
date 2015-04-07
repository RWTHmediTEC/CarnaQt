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

    /** \brief
      * Removes the represented `presets::MIPLayer` from the controlled \ref stage.
      * Emits the \ref removed and \ref changed signals.
      */
    void remove();

    /** \brief
      * Ascends the represented `presets::MIPLayer` on the controlled \ref stage.
      * Emits the \ref ascended and \ref changed signals.
      */
    void ascend();

    /** \brief
      * Descends the represented `presets::MIPLayer` on the controlled \ref stage.
      * Emits the \ref descended and \ref changed signals.
      */
    void descend();
    
signals:

    /** Emitted when the represented `presets::MIPLayer` is removed, ascended or
      * descended.
      */
    void changed();
    
    /** Emitted when the represented `presets::MIPLayer` is ascended.
      */
    void ascended( MIPControlLayer& );
    
    /** Emitted when the represented `presets::MIPLayer` is descended.
      */
    void descended( MIPControlLayer& );
    
    /** Emitted when the represented `presets::MIPLayer` is removed.
      */
    void removed( MIPControlLayer& );
    
private slots:

    void invalidate();

}; // MIPControlLayer



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MIPCONTROLLAYER_H_0874895466
