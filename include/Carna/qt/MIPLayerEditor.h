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

    const static unsigned int FUNCTION_REPLACE = 0;
    const static unsigned int FUNCTION_ADD     = 1;

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
    
    static std::string functionName( unsigned int functionIndex );
    
    static unsigned int functionIndex( const std::string& functionName );
    
    static unsigned int functionIndex( const base::BlendFunction& function );
    
    static const base::BlendFunction& function( unsigned int functionIndex );
    
public slots:

    void setFunction( int functionIndex );
    
    void setHuvMin( int huvMin );
    
    void setHuvMax( int huvMax );
    
    void setColor( const QColor& color );
    
signals:

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
