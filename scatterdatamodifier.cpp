/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "scatterdatamodifier.h"
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtCore/qmath.h>
#include <QtCore/qrandom.h>
#include <QtWidgets/QComboBox>

#include <QCategory3DAxis>

#define LANE_LINES 4

using namespace QtDataVisualization;

//#define RANDOM_SCATTER // Uncomment this to switch to random scatter

const int numberOfItems = 3600;
const float curveDivider = 3.0f;
const int lowerNumberOfItems = 900;
const float lowerCurveDivider = 0.75f;

ScatterDataModifier::ScatterDataModifier(Q3DScatter *scatter, Location *location)
    : m_graph(scatter),
      m_fontSize(40.0f),
      m_style(QAbstract3DSeries::MeshSphere),
      m_smooth(true),
      m_itemCount(lowerNumberOfItems),
      m_curveDivider(lowerCurveDivider),
      m_location(location)
{
    //! [0]
    m_graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
     m_graph->activeTheme()->setBackgroundEnabled(false);


    
    //! [0]

    //! [2]
    // QScatterDataProxy *left_lane_proxy = new QScatterDataProxy;
    // QScatterDataProxy *right_lane_proxy = new QScatterDataProxy;

    // QList<QScatterDataProxy> *lane_line_proxy = new QList<QScatterDataProxy>;

    QScatterDataProxy *lane_line_proxy[LANE_LINES];
    QScatter3DSeries *lane_line_series[LANE_LINES];
    for (int i = 0; i < LANE_LINES; i++)
    {
        lane_line_proxy[i] = new QScatterDataProxy;
        lane_line_series[i] = new QScatter3DSeries(lane_line_proxy[i]);
        lane_line_series[i]->setMesh(QAbstract3DSeries::Mesh::MeshPoint);
    }
    
    QScatterDataProxy *left_edge_proxy = new QScatterDataProxy;
    QScatterDataProxy *right_edge_proxy = new QScatterDataProxy;

    QScatter3DSeries *left_edge_series = new QScatter3DSeries(left_edge_proxy);
    QScatter3DSeries *right_edge_series = new QScatter3DSeries(right_edge_proxy);
    left_edge_series->setMesh(QAbstract3DSeries::Mesh::MeshPoint);
    right_edge_series->setMesh(QAbstract3DSeries::Mesh::MeshPoint);

    m_graph->addSeries(left_edge_series);
    m_graph->addSeries(right_edge_series);

    for (int i = 0; i < LANE_LINES; i++)
    {
        m_graph->addSeries(lane_line_series[i]);
    }

    m_graph->seriesList().at(0)->setMesh(QAbstract3DSeries::Mesh::MeshPoint);
    m_graph->seriesList().at(1)->setMesh(QAbstract3DSeries::Mesh::MeshPoint);

    m_graph->axisX()->setLabelFormat(QStringLiteral("%.2f X"));
    m_graph->axisY()->setLabelFormat(QStringLiteral("%.2f Y"));
    m_graph->axisZ()->setLabelFormat(QStringLiteral("%.2f Z"));

    m_graph->axisX()->setLabelAutoRotation(90);
    m_graph->axisY()->setLabelAutoRotation(90);
    m_graph->axisZ()->setLabelAutoRotation(90);

    m_graph->axisX()->setRange(0,192);
    m_graph->axisY()->setRange(-30,30);
    

    //! [2]

    QThread *thread = new QThread;
    m_location->moveToThread(thread);

    connect(thread, &QThread::started, m_location, &Location::handle_message);
    connect(m_location, SIGNAL (newMsg()), this, SLOT (recvMsg()));
    thread->start();

    //! [3]
    // addData();
    //! [3]
}

void ScatterDataModifier::recvMsg(){
    // qDebug() << "lat: " << m_location->lat << " lon: " << m_location->lon << " bea: " << m_location->bea;
    updateData();
}

void ScatterDataModifier::updateData(){
    // m_graph->axisX()->setTitle("X");
    // m_graph->axisY()->setTitle("Y");
    // m_graph->axisZ()->setTitle("Z");

    // QColor color_ll = QColor(0,0,0);

    for (int ll_idx = 0; ll_idx < LANE_LINES; ll_idx++)
    {
        QScatterDataArray *dataArray_ll = new QScatterDataArray;
        dataArray_ll->resize(TRAJECTORY_SIZE);
        QScatterDataItem *ptrToDataArray_ll = &dataArray_ll->first();

        float colorMult = qBound((double)m_location->lane_line_prob[ll_idx], 0.0, 1.0);

        // qDebug() << "colorMultLL: " << colorMult;
        
        QColor color_ll = QColor(colorMult, colorMult, colorMult);

       for (int i = 0; i < TRAJECTORY_SIZE; i++) {
            ptrToDataArray_ll->setPosition(QVector3D(m_location->laneX[ll_idx][i],
                                                m_location->laneY[ll_idx][i],
                                                m_location->laneZ[ll_idx][i]
                                                ));
            ptrToDataArray_ll++;
       }

        m_graph->seriesList().at(ll_idx)->dataProxy()->resetArray(dataArray_ll);
        m_graph->seriesList().at(ll_idx)->setBaseColor(color_ll);

    }
    
    QColor color_re = QColor(0,0,255);

    for (int re_idx = 0; re_idx < 2; re_idx++){

        QScatterDataArray *dataArray_re = new QScatterDataArray;
        dataArray_re->resize(TRAJECTORY_SIZE); //6 = ll + re;
        QScatterDataItem *ptrToDataArray_re = &dataArray_re->first();

        float colorMult = qBound((double)1.0-m_location->road_edge_std[re_idx], 0.0, 1.0);

        qDebug() << "colorMultRE: " << colorMult;
        
        color_re = QColor((int)255*colorMult, 0, 0);

        for (int i = 0; i < TRAJECTORY_SIZE; i++) {
            ptrToDataArray_re->setPosition(QVector3D(m_location->edgeX[re_idx][i],
                                                m_location->edgeY[re_idx][i],
                                                m_location->edgeZ[re_idx][i]
                                                ));
            ptrToDataArray_re++;
        }
        
        m_graph->seriesList().at(re_idx+4)->dataProxy()->resetArray(dataArray_re);
        m_graph->seriesList().at(re_idx+4)->setBaseColor(color_re);
    }
}



ScatterDataModifier::~ScatterDataModifier()
{
    delete m_graph;
}


void ScatterDataModifier::addData()
{
    // Configure the axes according to the data
    //! [4]
    m_graph->axisX()->setTitle("X");
    m_graph->axisY()->setTitle("Y");
    m_graph->axisZ()->setTitle("Z");

    //! [4]

    //! [5]
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(m_itemCount);
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    //! [5]

#ifdef RANDOM_SCATTER
    for (int i = 0; i < m_itemCount; i++) {
        ptrToDataArray->setPosition(randVector());
        ptrToDataArray++;
    }
#else
    //! [6]
    float limit = qSqrt(m_itemCount) / 2.0f;
    for (float i = -limit; i < limit; i++) {
        for (float j = -limit; j < limit; j++) {
            ptrToDataArray->setPosition(QVector3D(i + 0.5f,
                                                  qCos(qDegreesToRadians((i * j) / m_curveDivider)),
                                                  j + 0.5f));
            ptrToDataArray++;
        }
    }
    //! [6]
#endif

    //! [7]
    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
    //! [7]
}

//! [8]
void ScatterDataModifier::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        m_style = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        if (m_graph->seriesList().size())
            m_graph->seriesList().at(0)->setMesh(m_style);
    }
}

void ScatterDataModifier::setSmoothDots(int smooth)
{
    m_smooth = bool(smooth);
    QScatter3DSeries *series = m_graph->seriesList().at(0);
    series->setMeshSmooth(m_smooth);
}

void ScatterDataModifier::changeTheme(int theme)
{
    Q3DTheme *currentTheme = m_graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
}

void ScatterDataModifier::rotateXAxis(int degs){
    m_graph->scene()->activeCamera()->setWrapXRotation(true);
    qDebug() << "Xdegs: " << degs;
    m_graph->scene()->activeCamera()->setXRotation((float) degs);
}

void ScatterDataModifier::rotateYAxis(int degs){
    m_graph->scene()->activeCamera()->setWrapYRotation(true);
    qDebug() << "Ydegs: " << degs;
    m_graph->scene()->activeCamera()->setYRotation((float) degs);
}


void ScatterDataModifier::changePresetCamera()
{
    static int preset = Q3DCamera::CameraPresetFrontLow;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}

void ScatterDataModifier::changeLabelStyle()
{
    m_graph->activeTheme()->setLabelBackgroundEnabled(!m_graph->activeTheme()->isLabelBackgroundEnabled());
}

void ScatterDataModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setPointSizeF(m_fontSize);
    m_graph->activeTheme()->setFont(newFont);
}

void ScatterDataModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);
    emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
}

void ScatterDataModifier::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
}

void ScatterDataModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled((bool)enabled);
}

void ScatterDataModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled((bool)enabled);
}
//! [8]

void ScatterDataModifier::toggleItemCount()
{
    if (m_itemCount == numberOfItems) {
        m_itemCount = lowerNumberOfItems;
        m_curveDivider = lowerCurveDivider;
    } else {
        m_itemCount = numberOfItems;
        m_curveDivider = curveDivider;
    }
    m_graph->seriesList().at(0)->dataProxy()->resetArray(0);
    addData();
}

QVector3D ScatterDataModifier::randVector()
{
    return QVector3D(
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f,
                (float)(QRandomGenerator::global()->bounded(100)) / 100.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 100.0f,
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f -
                (float)(QRandomGenerator::global()->bounded(100)) / 2.0f);
}
