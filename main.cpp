#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QThread>

#include <QMapboxGL>

#include "u2_window.h"
#include "mapwindow.hpp"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMapboxGLSettings settings;
    settings.setCacheDatabasePath("/tmp/mbgl-cache.db");
    settings.setCacheDatabaseMaximumSize(20 * 1024 * 1024);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.process(app);

    U2Window u2window;
    
    MapWindow mapwindow(settings);
    
    u2window.resize(800, 600);
    u2window.show();

    mapwindow.resize(800, 600);
    mapwindow.show();


    return app.exec();
    
}
