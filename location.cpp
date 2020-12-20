// #include <QObject>
// #include <QThread>
// #include <QReadWriteLock>

// #include "../repos/cereal/messaging/messaging.hpp"

#include "location.h"

Location::Location(){
    qDebug() << "Location constuct()!";
    m_sm = new SubMaster({"controlsState", "gpsLocationExternal"});
    // m_sm = new SubMaster({"thermal","controlsState"});

}

Location::~Location(){
}

void Location::handle_message(){
    qDebug() << "handle_message()!";
    SubMaster sm = *(m_sm);

    // GpsLocationData
    

    while (1){
        if (sm.update(0) > 0){
            qDebug() << "got update";
            if (sm.updated("controlsState")) {

                // cereal::Event::Reader
                auto event = sm["controlsState"];
                // event.getControlsState();
                controls_state = event.getControlsState();
                vel = controls_state.getVEgo();

                
                // getVEgo()
                emit newMsg();
                // auto data = sm["radarState"].getRadarState();
                // scene.lead_data[0] = data.getLeadOne();
                // scene.lead_data[1] = data.getLeadTwo();
            }

            if (sm.updated("gpsLocationExternal")){
                auto event = sm["gpsLocationExternal"];
                gps_state = event.getGpsLocationExternal();
                lat = gps_state.getLatitude();
                lon = gps_state.getLongitude();
                bea = gps_state.getBearing();
                qDebug() << "got GPS data!";
            }
        }
    }
}
