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

#ifndef COLORPICKER_H_0874895466
#define COLORPICKER_H_0874895466

/** \file   ColorPicker.h
  * \brief  Defines \ref Carna::qt::ColorPicker.
  */

#include <Carna/qt/CarnaQt.h>
#include <QFrame>
#include <QColor>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ColorPicker
// ----------------------------------------------------------------------------------

/** \brief
  * Implements widget for selecting colors with or without alpha values.
  *
  * \ingroup Widgets
  *
  * Examples:
  *
  * \image html ColorPicker01.png
  *
  * The alpha value can be adjusted quickly by using the scroll wheel.
  *
  * The painting routine is delegated to an instance of \ref ColorPickerPainter.
  *
  * \author Leonid Kostrykin
  * \date   28.7.11 - 2.4.15
  */
class CARNAQT_LIB ColorPicker : public QFrame
{

    Q_OBJECT

public:

    /** \brief
      * Instantiates.
      */
    explicit ColorPicker
        ( const QColor& initial
        , bool ignoreAlpha = false
        , QWidget* parent = nullptr );

    /** \brief
      * Fixed size is used.
      */
    virtual QSize sizeHint() const;

    /** \brief
      * Tells the currently set color.
      */
    const QColor& getColor() const
    {
        return color;
    }

protected:

    /** \brief
      * Opens a color picking dialog.
      */
    virtual void mouseReleaseEvent( QMouseEvent* );

    /** \brief
      * Paints the widget.
      */
    virtual void paintEvent( QPaintEvent* );

    /** \brief
      * Manipulates the alpha value.
      */
    virtual void wheelEvent( QWheelEvent* ev );

signals:

    /** \brief
      * The current color may have changed.
      */
    void colorChanged();

    /** \brief
      * The current color may have changed to the given one.
      */
    void colorChanged( const QColor& );

public slots:

    /** \brief
      * Sets current color.
      */
    void setColor( const QColor& color );

private:

    QColor color;
    
    const bool ignoreAlpha;

}; // ColorPicker



}  // namespace Carna :: qt

}  // namespace Carna

#endif // COLORPICKER_H_0874895466
