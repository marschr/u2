#include "u2_window.h"
#include <QDebug>
#include <QVBoxLayout>

#include "location.h"


GLWindow::GLWindow(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "GLWindow ctor";

}
GLWindow::~GLWindow() {
    qDebug() << "GLWindow dtor";
}

U2Window::U2Window()
    : location(nullptr)
{
    qDebug() << "U2Window ctor";


    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->setMargin(0);

    glWindow = new GLWindow(this);
    main_layout->addWidget(glWindow, 0, 0);

    //location stuff
    QThread *thread = new QThread;
    location = new Location();
    location->moveToThread(thread);
    connect(thread, &QThread::started, location, &Location::handle_message);
    connect(location, SIGNAL (newMsg()), this, SLOT (recvMsg()));
    thread->start();

    setLayout(main_layout);
    setStyleSheet(R"(background-color: red;)");
}

void U2Window::recvMsg(){
    qDebug() << "lat: " << location->lat << " lon: " << location->lon << " bea: " << location->bea;
}

U2Window::~U2Window(){
    qDebug() << "U2Window dtor";
}