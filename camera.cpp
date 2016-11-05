#include "camera.h"

Camera::Camera()
{
    view.lookAt(QVector3D(0,0,0), QVector3D(0,0,0), QVector3D(0,1,0));
    projection.setToIdentity();
}

Camera::Camera(const QVector3D &eye, const QVector3D &center, const QVector3D &up)
{
    view.lookAt(eye, center, up);
    projection.setToIdentity();
}

void Camera::rotate(float angle, const QVector3D &vec)
{
    view.rotate(angle, vec);
}

void Camera::rotate(QQuaternion &quat)
{
    view.rotate(quat);
}

void Camera::perspective(float verticalAngle, float aspectRation, float nearPlane, float farPlane)
{
    projection.setToIdentity();
    projection.perspective(verticalAngle, aspectRation, nearPlane, farPlane);
}
