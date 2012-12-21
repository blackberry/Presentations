#include "BarcodeScannerApp.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>

using ::bb::cascades::Application;

int main(int argc, char **argv)
{
    //-- this is where the server is started etc
    Application app(argc, argv);
    
    //-- localization support
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString( "BarcodeScanne_%1" ).arg( locale_string );
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator( &translator );
    }
    
    new BarcodeScannerApp(&app);
    
    //-- we complete the transaction started in the app constructor and start the client event loop here
    return Application::exec();
    //-- when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
