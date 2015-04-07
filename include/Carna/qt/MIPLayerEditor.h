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

#ifndef MIPLAYEREDITOR_0874895466
#define MIPLAYEREDITOR_0874895466

/** \file   MIPLayerEditor.h
  * \brief  Defines \ref Carna::qt::MIPLayerEditor.
  */

#include <Carna/qt/CarnaQt.h>
#include <QFrame>

class QColor;
class QSpinBox;
class QComboBox;

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MIPLayerEditor
// ----------------------------------------------------------------------------------

/** \brief  Defines `presets::MIPLayer` editing widget.
  *
  * \author Leonid Kostrykin
  * \date   26.7.11 - 3.4.15
  */
class CARNAQT_LIB MIPLayerEditor : public QFrame
{

    Q_OBJECT

public:

    const static unsigned int FUNCTION_REPLACE = 0; ///< Indentifies `presets::MIPLayer::LAYER_FUNCTION_REPLACE`.
    const static unsigned int FUNCTION_ADD     = 1; ///< Indentifies `presets::MIPLayer::LAYER_FUNCTION_ADD`.

    /** \brief
      * Instantiates.
      */
    explicit MIPLayerEditor( presets::MIPLayer& layer );

    /** \brief
      * Returns the optimal size.
      */
    virtual QSize sizeHint() const;

    /** \brief
      * References the layer that is edited by this widget.
      */
    presets::MIPLayer& layer;
    
    /** \brief
      * Tells human-readable name for the layer function \a functionIndex identifies.
      */
    static std::string functionName( unsigned int functionIndex );
    
    /** \brief
      * Identifies the layer function by its human-readable \a functionName.
      */
    static unsigned int functionIndex( const std::string& functionName );
    
    /** \brief
      * Identifies the layer function by its \a function parameters.
      */
    static unsigned int functionIndex( const base::BlendFunction& function );
    
    /** \brief
      * References the layer function \a functionIndex identifies.
      */
    static const base::BlendFunction& function( unsigned int functionIndex );
    
public slots:

    /** \brief
      * Updates the layer function on \ref layer and emits \ref changed.
      */
    void setFunction( int functionIndex );
    
    /** \brief
      * Delegates to \ref layer and emits \ref changed.
      */
    void setHuvMin( int huvMin );
    
    /** \brief
      * Delegates to \ref layer and emits \ref changed.
      */
    void setHuvMax( int huvMax );
    
    /** \brief
      * Delegates to \ref layer and emits \ref changed.
      */
    void setColor( const QColor& color );
    
signals:

    /** \brief
      * Emitted whenever \ref layer changes.
      */
    void changed();

private:

    QLabel         * const laHuvMin;
    QLabel         * const laHuvMax;
    QSpinBox       * const sbHuvMin;
    QSpinBox       * const sbHuvMax;
    WideColorPicker* const colorPicker;
    QComboBox      * const cbFunction;

}; // MIPLayerEditor



}  // namespace Carna :: qt

}  // namespace Carna

#endif // MIPLAYEREDITOR_0874895466
