#define TRAJECTORY_SIZE 33

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

        cereal::ModelDataV2::XYZTData::Reader lane_lines;
        cereal::ModelDataV2::XYZTData::Reader road_edges;
        float lane_line_prob[4];
        float road_edge_std[2];
        
        //should L/R lines/edges + trajectory xyz
        float laneX[4][TRAJECTORY_SIZE];
        float laneY[4][TRAJECTORY_SIZE];
        float laneZ[4][TRAJECTORY_SIZE];

        float edgeX[2][TRAJECTORY_SIZE];
        float edgeY[2][TRAJECTORY_SIZE];
        float edgeZ[2][TRAJECTORY_SIZE];

    public slots:
        void handle_message();

    signals:
        void newMsg();

    // private:        
    //     void getLocation();
};

#endif