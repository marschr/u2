#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QThread>

#include <QMapboxGL>

#include "u2_window.h"
#include "mapwindow.hpp"
#include "location.h"

//maybe encapsulate outside here
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtGui/QScreen>

//move to scatter.h later on
#include "scatterdatamodifier.h"
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMapboxGLSettings settings;
    settings.setCacheDatabasePath("/tmp/mbgl-cache.db");
    settings.setCacheDatabaseMaximumSize(20 * 1024 * 1024);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.process(app);

    //scatter
    QtDataVisualization::Q3DScatter *graph = new QtDataVisualization::Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);
    
    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 3, screenSize.height() / 3));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    //scatter

    
    MapWindow mapwindow(settings);

    
    mapwindow.resize(800, 600);
    mapwindow.show();
    Location *location = new Location();
    ScatterDataModifier *modifier = new ScatterDataModifier(graph, location);

    
    U2Window *u2window = new U2Window();
    u2window->resize(800, 600);
    u2window->show();
    widget->show();


    return app.exec();
    
}
