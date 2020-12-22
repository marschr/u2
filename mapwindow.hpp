#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QMapboxGL>

#include <QOpenGLWidget>
#include <QPropertyAnimation>
#include <QScopedPointer>
#include <QtGlobal>

#include "location.h"


class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

class MapWindow : public QOpenGLWidget
{
    Q_OBJECT

public:
    MapWindow(const QMapboxGLSettings &, Location *location);
    ~MapWindow();

    void selfTest();

public Q_SLOTS:
    void recvMsg();

protected slots:
    void animationValueChanged();
    void animationFinished();

private:
    void changeStyle();
    qreal pixelRatio();

    // QWidget implementation.
    void keyPressEvent(QKeyEvent *ev) final;
    void mousePressEvent(QMouseEvent *ev) final;
    void mouseMoveEvent(QMouseEvent *ev) final;
    void wheelEvent(QWheelEvent *ev) final;

    // Q{,Open}GLWidget implementation.
    void initializeGL() final;
    void paintGL() final;

    QPointF m_lastPos;

    QMapboxGLSettings m_settings;
    QScopedPointer<QMapboxGL> m_map;

    QPropertyAnimation *m_bearingAnimation;
    QPropertyAnimation *m_zoomAnimation;

    unsigned m_animationTicks = 0;
    unsigned m_frameDraws = 0;

    QVariant m_symbolAnnotationId;
    QVariant m_lineAnnotationId;
    QVariant m_fillAnnotationId;

    QVariant m_lineAnnotationIdL;
    QVariant m_lineAnnotationIdR;

    bool m_sourceAdded = false;
    bool m_source2Added = false;
    bool m_enableRecv = false;
    bool m_firstRun = true;
    Location *m_location;
    QMapbox::Coordinate m_lastCoords[2];
};

#endif
