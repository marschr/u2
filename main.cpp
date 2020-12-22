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

#include <QSlider>


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

    QSlider *sliderX = new QSlider(Qt::Orientation::Horizontal);
    sliderX->setRange(-360,360);
    QSlider *sliderY = new QSlider(Qt::Orientation::Horizontal);
    sliderY->setRange(-360,360);


    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();

    vLayout->addWidget(sliderX);
    vLayout->addWidget(sliderY);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    //scatter
       
   
    Location *location = new Location();

    MapWindow *mapwindow = new MapWindow(settings, location);
    mapwindow->setGeometry(2000,900,1200,1200);
    // mapwindow->resize(800, 600);

    ScatterDataModifier *modifier = new ScatterDataModifier(graph, location);

    QObject::connect(sliderX, &QSlider::valueChanged, modifier,
                     &ScatterDataModifier::rotateXAxis);
    QObject::connect(sliderY, &QSlider::valueChanged, modifier,
                     &ScatterDataModifier::rotateYAxis);


    QThread *thread = new QThread;
    location->moveToThread(thread);

    QObject::connect(thread, &QThread::started, location, &Location::handle_message);
    QObject::connect(location, SIGNAL (newMsg()), modifier, SLOT (recvMsg()));

    QObject::connect(thread, &QThread::started, location, &Location::handle_message);
    QObject::connect(location, SIGNAL (newMsg()), mapwindow, SLOT (recvMsg()));


    thread->start();

    
    U2Window *u2window = new U2Window();
    // u2window->resize(800, 600);
    // u2window->show();
    widget->setGeometry(2000,70,container->minimumSize().width(), container->minimumSize().height());
    widget->show();
    mapwindow->show();


    return app.exec();
    
}
