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

#ifndef EXPANDABLEGROUPBOX_H_0874895466
#define EXPANDABLEGROUPBOX_H_0874895466

/** \file   ExpandableGroupBox.h
  * \brief  Defines \ref Carna::qt::ExpandableGroupBox.
  */

#include <Carna/Carna.h>
#include <QFrame>
#include <memory>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ExpandableGroupBox
// ----------------------------------------------------------------------------------

/** \brief
  * Widget container that can hide its contents in order to save UI space.
  *
  * \image html ExpandableGroupBox01.png
  *
  * \ingroup Widgets
  *
  * The child `%ExpandableGroupBox` uses by default is provided with a `QVBoxLayout`.
  *
  * Usage example:
  *
  *     \code
  *     ExpandableGroupBox* const gbOptions = new ExpandableGroupBox( "Options" );
  *     gbOptions->child()->layout()->addWidget( someWidget1 );
  *     gbOptions->child()->layout()->addWidget( someWidget2 );
  *     \endcode
  *
  * Another example:
  *     \code
  *     ExpandableGroupBox* const gbOptions = new ExpandableGroupBox( "Options" );
  *     QFormLayout* const options = new QFormLayout();
  *     gbOptions->child()->setLayout( options );
  *     gbOptions->child()->layout()->setContentsMargins( 0, 0, 0, 0 );
  *     options->addRow( "Option 1", someWidget1 );
  *     options->addRow( "Option 2", someWidget2 );
  *     \endcode
  *
  * \author Leonid Kostrykin
  * \date   12.9.12 - 2.4.15
  */
class CARNAQT_LIB ExpandableGroupBox : public QFrame
{

    Q_OBJECT

public:

    /** \brief
      * Instantiates, either \a expended or collapsed initially.
      */
    explicit ExpandableGroupBox( const QString& text = "", bool expended = false, QWidget* parent = nullptr );

    /** \brief
      * Deletes.
      */
    virtual ~ExpandableGroupBox();

    /** \brief
      * Tells the group box' current label text.
      */
    const QString& text() const;

    /** \brief
      * Tells whether the group box is currently expended.
      */
    bool isExpended() const;
    
    /** \brief
      * Tells whether the group box is currently collapsed (not expended).
      */
    bool isCollapsed() const;

    /** \brief
      * Removes this group box' contents.
      *
      * The ownership of the `QWidget` representing the contents is transfered to the
      * caller of this method, so you will have to either put the returned widget in
      * some other layout or release it's memory, like:
      * 
      *     \code
      *     delete gb->takeChild();
      *     \endcode
      *
      * The returned value is `nullptr` if the group box does not have a child.
      */
    QWidget* takeChild();

    /** \brief
      * References the contents of this group box.
      */
    QWidget* child() const;


signals:

    /** \brief
      * The group box has been expended.
      */
    void expanded();
    
    /** \brief
      * The group box has been collapsed.
      */
    void collapsed();


public slots:

    /** \brief
      * Sets the group box' label text.
      */
    void setText( const QString& );

    /** \brief
      * Expends or collapses the group box.
      *
      * Emits either the \ref expended or the \ref collapsed signal.
      */
    void setExpended( bool );

    /** \brief
      * Expends the group box.
      *
      * If the group box is expended already, nothing happens.
      *
      * Emits the \ref expended signal.
      */
    void expend();
    
    /** \brief
      * Collapses the group box.
      *
      * If the group box is collapsed already, nothing happens.
      *
      * Emits the \ref collapsed signal.
      */
    void collapse();

    /** \brief
      * Switches the group box from 'collapsed' into 'expended' state and vice-versa.
      *
      * Emits either the \ref expended or the \ref collapsed signal.
      */
    void toggle();

    /** \brief
      * Sets this group box' child.
      *
      * If the group box already has a child, it will be released first.
      */
    void setChild( QWidget* );

private:

    class Details;
    std::unique_ptr< Details > pimpl;

private slots:

    void toggle( const QString& );

}; // ExpandableGroupBox



}  // namespace Carna :: qt

}  // namespace Carna

#endif // EXPANDABLEGROUPBOX_H_0874895466
