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

#include <Carna/qt/Application.h>
#include <Carna/base/CarnaException.h>
#include <Carna/base/Log.h>
#include <QMessageBox>
#include <QPushButton>

namespace Carna
{

namespace qt
{



// ----------------------------------------------------------------------------------
// QDebugLogWriter
// ----------------------------------------------------------------------------------

class QDebugLogWriter : public base::Log::TextWriter
{

protected:

    virtual void writeLine( base::Log::Severity severity, const std::string& msg ) const override;

};


void QDebugLogWriter::writeLine( base::Log::Severity severity, const std::string& msg ) const
{
    switch( severity )
    {

    case base::Log::fatal:
    case base::Log::error:
        qCritical( "%s", msg.c_str() );
        break;

    case base::Log::warning:
        qWarning( "%s", msg.c_str() );
        break;

    case base::Log::debug:
        qDebug( "%s", msg.c_str() );
        break;

    case base::Log::verbose:
        qDebug( "%s", msg.c_str() );
        break;

    }
}



// ----------------------------------------------------------------------------------
// Application
// ----------------------------------------------------------------------------------

Application::Application( int& argc, char** argv )
    : QApplication( argc, argv )
{
    base::Log::instance().setWriter( new QDebugLogWriter() );
}


Application::~Application()
{
    /* We need to do this as long as 'QApplication' is still alive, s.t. 'QDebug' is
     * also still available.
     */
    base::Log::instance().shutdown();
}


bool Application::notify( QObject* receiver, QEvent* ev )
{
    try
    {
        try
        {
            return QApplication::notify( receiver, ev );
        }
        catch( const std::runtime_error& ex )
        {
            throw base::CarnaException( ex );
        }
        catch( const std::logic_error& ex )
        {
            throw base::CarnaException( ex );
        }
        catch( const std::bad_alloc& ex )
        {
            throw base::CarnaException
                ( "Fatal Error"
                , "Not enough memory to finish operation."
                , ex.what() );
        }
        catch( const std::exception& ex )
        {
            throw base::CarnaException
                ( "Fatal Error"
                , "Fatal error: An unknown exception has occurred."
                , ex.what() );
        }
        catch( const base::CarnaException& )
        {
            throw;
        }
        catch( ... )
        {
            throw base::CarnaException( "Fatal Error", "Fatal error: An unknown exception has occurred." );
        }
    }
    catch( base::CarnaException& ex )
    {
        base::Log::instance().record( base::Log::fatal, ex.message );
        base::Log::instance().record( base::Log::fatal, "Details: " + ex.details );
        
        QMessageBox msgBox;

        msgBox.setWindowModality( Qt::ApplicationModal );
        msgBox.setModal( true );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.setWindowTitle( QString::fromStdString( ex.type ) );
        msgBox.setText( QString::fromStdString( ex.message ) );
        msgBox.setDetailedText( QString::fromStdString( ex.details ) );
        msgBox.setStandardButtons( QMessageBox::Ignore );
        msgBox.setDefaultButton( QMessageBox::Ignore );
        msgBox.setEscapeButton( QMessageBox::Ignore );

        QPushButton* const exitButton = msgBox.addButton( "Exit", QMessageBox::ResetRole );

        msgBox.exec();

        if( msgBox.clickedButton() == exitButton )
        {
            ::exit( EXIT_FAILURE );
        }
    }
    return false;
}



}  // namespace Carna :: qt

}  // namespace Carna

