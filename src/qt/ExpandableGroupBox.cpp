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

#include <Carna/qt/ExpandableGroupBox.h>
#include <QLabel>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// ExpandableGroupBox :: Details
// ----------------------------------------------------------------------------------

class ExpandableGroupBox::Details
{

    NON_COPYABLE

public:

    Details();

    QLabel* const caption;

    QString text;

    bool expended;

    QVBoxLayout* const global;

    QWidget* child;

    void updateCaption();

private:

    QMargins expendedCaptionMargins, collapsedCaptionMargins;

}; // ExpandableGroupBox :: Details


ExpandableGroupBox::Details::Details()
    : caption( new QLabel() )
    , expended( false )
    , global( new QVBoxLayout() )
    , child( nullptr )
{
    expendedCaptionMargins = collapsedCaptionMargins = caption->contentsMargins();
    expendedCaptionMargins.setBottom( collapsedCaptionMargins.bottom() + 5 );
}


void ExpandableGroupBox::Details::updateCaption()
{
    const QString prefix = expended ? "&#9660;" : "&#9658;";
    caption->setText( prefix + " <a href=\"#\">" + text + "</a>" );
    caption->setContentsMargins( expended ? expendedCaptionMargins : collapsedCaptionMargins );
}



// ----------------------------------------------------------------------------------
// ExpandableGroupBox
// ----------------------------------------------------------------------------------

ExpandableGroupBox::ExpandableGroupBox( const QString& text, bool expended, QWidget* parent )
    : QFrame( parent )
    , pimpl( new Details() )
{
    this->setText( text );
    this->setExpended( expended );

    this->setLayout( pimpl->global );
    pimpl->global->addWidget( pimpl->caption );
    
    pimpl->caption->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );

    connect( pimpl->caption, SIGNAL( linkActivated( const QString& ) ), this, SLOT( toggle( const QString& ) ) );

    this->setObjectName( "ExpandableGroupBox" );
    this->setStyleSheet( "QWidget#ExpandableGroupBox{ border: 1px solid " + QPalette().light().color().name() + "; border-radius: 5px; }" );

    this->setChild( new QWidget() );
}


ExpandableGroupBox::~ExpandableGroupBox()
{
}


void ExpandableGroupBox::setText( const QString& text )
{
    pimpl->text = text;
    pimpl->updateCaption();
}


const QString& ExpandableGroupBox::text() const
{
    return pimpl->text;
}


void ExpandableGroupBox::setExpended( bool expended )
{
    pimpl->expended = expended;
    pimpl->updateCaption();

    if( pimpl->child )
    {
        pimpl->child->setVisible( pimpl->expended );
    }
}


bool ExpandableGroupBox::isExpended() const
{
    return pimpl->expended;
}


bool ExpandableGroupBox::isCollapsed() const
{
    return !isExpended();
}


void ExpandableGroupBox::expend()
{
    setExpended( true );
}


void ExpandableGroupBox::collapse()
{
    setExpended( false );
}


QWidget* ExpandableGroupBox::takeChild()
{
    if( !pimpl->child )
    {
        return nullptr;
    }

    pimpl->child->hide();
    pimpl->global->removeWidget( pimpl->child );

    QWidget* const child = pimpl->child;
    pimpl->child = nullptr;
    return child;
}


void ExpandableGroupBox::setChild( QWidget* child )
{
    if( pimpl->child )
    {
        delete takeChild();
    }

    pimpl->child = child;
    if( pimpl->child->layout() )
    {
        pimpl->child->layout()->setContentsMargins( 0, 0, 0, 0 );
    }

    if( isCollapsed() )
    {
        pimpl->child->setVisible( false );
    }
    pimpl->global->addWidget( pimpl->child );
    if( isExpended() )
    {
        pimpl->child->setVisible( true );
    }
}


void ExpandableGroupBox::toggle()
{
    setExpended( !isExpended() );
}


void ExpandableGroupBox::toggle( const QString& )
{
    toggle();
}


QWidget* ExpandableGroupBox::child() const
{
    return pimpl->child;
}



}  // namespace Carna :: qt

}  // namespace Carna
