#include "u2_window.h"
#include <QDebug>

#include "location.h"

U2Window::U2Window()
    : location(nullptr)
{
    qDebug() << "ctor";

    QThread *thread = new QThread;
    location = new Location();
    location->moveToThread(thread);

    connect(thread, &QThread::started, location, &Location::handle_message);
    connect(location, SIGNAL (newMsg()), this, SLOT (recvMsg()));

    thread->start();
}

void U2Window::recvMsg(){
    qDebug() << "lat: " << location->lat << " lon: " << location->lon << " bea: " << location->bea;
}

U2Window::~U2Window(){
    qDebug() << "dtor";
}