#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera
{
public:
    Camera();
    Camera(const QVector3D &eye, const QVector3D &center, const QVector3D &up);
    void perspective(float verticalAngle, float aspectRation, float nearPlane, float farPlane);
    void rotate(float angle, const QVector3D &vec);
    void rotate(QQuaternion &quat);
    QMatrix4x4 getView() { return this->view; }
    QMatrix4x4 getProjection() { return this->projection; }

protected:
    QMatrix4x4 view;
    QMatrix4x4 projection;
};

#endif // CAMERA_H
