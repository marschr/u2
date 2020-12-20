TEMPLATE = app
INCLUDEPATH += .
INCLUDEPATH += ./include

# Input
SOURCES += main.cpp \
            u2_window.cpp \
            location.cpp \
            mapwindow.cpp
           
HEADERS = u2_window.h \
            location.h \
            mapwindow.hpp

#RESOURCES += rendercontrol.qrc

CONFIG += c++14
CONFIG += debug

QT += core qml quick widgets opengl network positioning location sql openglextensions websockets



#BASEDIR = "../../"
#PHONELIBS = $$BASEDIR"phonelibs"

OPDIR="../openpilot/"

CEREALDIR="../repos/commaai/cereal/"

CAPNPDIR="../repos/capnproto/"

INCLUDEPATH += $$CEREALDIR 

LIBS += -L$$CEREALDIR -L$$CAPNPDIR"c++/src/capnp" -L$$CAPNPDIR"c++/src/kj" #-L$$OPDIR"cereal"

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil -lswscale -lzmq -lbz2 -ldl -lcapnp -lkj
LIBS += $$CEREALDIR/libcereal_shared.so  $$CEREALDIR/libmessaging_shared.so
LIBS += -lqmapboxgl


#INCLUDEPATH = $$PHONELIBS/capnp-cpp/include #../clib/ #$$PHONELIBS/yaml-cpp/include

#unix:!macx {
#  LIBS += -L$$PHONELIBS/capnp-cpp/x64/lib -Wl,-rpath=$$PHONELIBS/capnp-cpp/x64/lib #-L$$PHONELIBS/yaml-cpp/x64/lib
#}

#macx: {
#  LIBS += -L$$PHONELIBS/capnp-cpp/mac/lib #-L$$PHONELIBS/yaml-cpp/mac/lib
#}


#INCLUDEPATH += $$PHONELIBS/capnp-cpp/include $$BASEDIR $$BASEDIR/cereal/messaging #$$PHONELIBS/yaml-cpp/include

#LIBS += -lbz2 $$BASEDIR/cereal/libmessaging.a -lzmq -L$$BASEDIR/cereal -L$$BASEDIR/cereal/messaging -lcereal



#[common, 'zmq', 'czmq', 'capnp', 'kj', 'm', cereal, messaging, gpucommon, visionipc]