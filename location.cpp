// #include <QObject>
// #include <QThread>
// #include <QReadWriteLock>

// #include "../repos/cereal/messaging/messaging.hpp"

#include "location.h"

Location::Location(){
    qDebug() << "Location constuct()!";
    m_sm = new SubMaster({"modelV2", "controlsState", "gpsLocationExternal"});
    // m_sm = new SubMaster({"thermal","controlsState"});

}

Location::~Location(){
}

void Location::handle_message(){
    qDebug() << "handle_message()!";
    SubMaster sm = *(m_sm);

    // GpsLocationData


    long frameIdx;
    

    while (1){
        
        if (sm.update(0) > 0){
            
        // qDebug() << "got update: while";
            if (sm.updated("modelV2")) {
                // qDebug() << "got update: modelV2";

                auto event = sm["modelV2"];
                model = event.getModelV2();
                
                for (int re_idx = 0; re_idx < 2; re_idx++) {
                    if (model.getRoadEdgeStds().size() > re_idx) {
                        road_edge_std[re_idx] = model.getRoadEdgeStds()[re_idx];

                            // qDebug() << "RE IF";

                            road_edges = model.getRoadEdges()[re_idx];
                            for (int i = 0; i < TRAJECTORY_SIZE; i++) {
                                 edgeX[re_idx][i] = road_edges.getX()[i];
                                 edgeY[re_idx][i] = road_edges.getY()[i];
                                 edgeZ[re_idx][i] = road_edges.getZ()[i];
                                // qDebug() << "re_idx: " << re_idx << "; x: " << road_edges.getX()[i] << "; y: " << road_edges.getY()[i] << "; z: " << road_edges.getZ()[i];

                            }                    
                    } else {
                        qDebug() << "got update: road_edge_std1.0";
                        road_edge_std[re_idx] = 1.0;
                    }
                }

                // //csv printer
                // frameIdx++;
                // for (int i = 0; i < TRAJECTORY_SIZE; i++) {
                //     qDebug() << frameIdx << "," << edgeX[i] << "," << edgeY[i] << "," << edgeZ[i];
                // }
                // QVector3D(edgeX,edgeY,edgeZ);
                
                
            }


            
            if (sm.updated("controlsState")) {
                // qDebug() << "got update: controlsState";

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
                // qDebug() << "got update: gpsLocationExternal";

                auto event = sm["gpsLocationExternal"];
                gps_state = event.getGpsLocationExternal();
                lat = gps_state.getLatitude();
                lon = gps_state.getLongitude();
                bea = gps_state.getBearing();
                // qDebug() << "got GPS data!";
            }
        }
    }
}
