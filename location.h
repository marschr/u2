#ifndef LOCATION_H
#define LOCATION_H
#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include <QDebug>

#include "../repos/commaai/cereal/messaging/messaging.hpp"

QT_USE_NAMESPACE

class Location : public QObject {
Q_OBJECT
    public:
        Location();
        ~Location();
        
        // // sockets
        SubMaster *m_sm;

        cereal::ControlsState::Reader controls_state;
        cereal::GpsLocationData::Reader gps_state;

        cereal::ModelDataV2::Reader model;
    
        float lat;
        float lon;
        float bea;
        float vel;

        cereal::ModelDataV2::XYZTData::Reader road_edges;
        float road_edge_std[2];

    public slots:
        void handle_message();

    signals:
        void newMsg();

    // private:        
    //     void getLocation();
};

#endif