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

#include <Carna/qt/MIPControlLayer.h>
#include <Carna/qt/MIPLayerEditor.h>
#include <Carna/presets/MIPLayer.h>
#include <Carna/presets/MIPStage.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// MIPControlLayer
// ----------------------------------------------------------------------------------

MIPControlLayer::MIPControlLayer( presets::MIPStage& stage, presets::MIPLayer& layer, QWidget* parent )
    : QWidget( parent )
    , stage( stage )
    , editor( *new MIPLayerEditor( layer ) )
{
    QHBoxLayout* master = new QHBoxLayout();
    QVBoxLayout* buttons = new QVBoxLayout();

    QPushButton* buAscend  = new QPushButton( QChar(  8593 ) );
    QPushButton* buDelete  = new QPushButton( QChar( 10005 ) );
    QPushButton* buDescend = new QPushButton( QChar(  8595 ) );

    buAscend ->setFixedSize( 20, 20 );
    buDelete ->setFixedSize( 20, 20 );
    buDescend->setFixedSize( 20, 20 );

    buttons->addWidget( buAscend );
    buttons->addWidget( buDelete );
    buttons->addWidget( buDescend );

    connect( buAscend , SIGNAL( clicked() ), this, SLOT(  ascend() ) );
    connect( buDelete , SIGNAL( clicked() ), this, SLOT(  remove() ) );
    connect( buDescend, SIGNAL( clicked() ), this, SLOT( descend() ) );

    master->addLayout( buttons );
    master->addWidget( &editor );
    
    connect( &editor, SIGNAL( changed() ), this, SLOT( invalidate() ) );
    
    master ->setContentsMargins( 0, 0, 0, 0 );
    buttons->setContentsMargins( 0, 0, 0, 0 );

    this->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    this->setLayout( master );
}


QSize MIPControlLayer::sizeHint() const
{
    const unsigned int w = editor.sizeHint().width() + 20;
    const unsigned int h = editor.sizeHint().height();
    return QSize( w, h );
}


void MIPControlLayer::ascend()
{
    stage.ascendLayer( editor.layer );
    emit ascended( *this );
    invalidate();
}


void MIPControlLayer::descend()
{
    bool hasPredecessor = false;
    std::size_t layerIdx;
    for( layerIdx = 1; layerIdx < stage.layersCount(); ++layerIdx )
    {
        if( &editor.layer == &stage.layer( layerIdx ) )
        {
            hasPredecessor = true;
            --layerIdx;
            break;
        }
    }
    
    if( hasPredecessor )
    {
        const presets::MIPLayer& successor = stage.layer( layerIdx );
        stage.ascendLayer( successor );
        emit descended( *this );
        invalidate();
    }
}


void MIPControlLayer::remove()
{
    delete stage.removeLayer( editor.layer );
    this->deleteLater();
    emit removed( *this );
    invalidate();
}


void MIPControlLayer::invalidate()
{
    emit changed();
}



}  // namespace Carna :: qt

}  // namespace Carna
