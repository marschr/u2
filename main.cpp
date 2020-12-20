#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QThread>

#include "u2_window.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.process(app);

    U2Window u2window;
    
    u2window.resize(800, 600);
    u2window.show();

    return app.exec();
    
}
