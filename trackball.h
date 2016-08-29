#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <QVector3D>
#include <QQuaternion>
#include <QTime>

#include <cmath>

class TrackBall
{
public:
    TrackBall();

    void mouseMove(const QPointF &p);
    void mousePress(const QPointF &p);
    void mouseRelease(const QPointF &p);
    void resizeViewPort(int width, int height);

    QQuaternion getRotation();

private:
    QQuaternion rotation;
    QVector3D axis;
    QVector3D lastPos3D;
    QTime lastTime;
    bool trackingMouse;
    double velocity;
    double viewPortWidth, viewPortHeight;

    const double RAD2DEG;

    QVector3D mousePosTo3D(const QPointF &p);

};

#endif // TRACKBALL_H
