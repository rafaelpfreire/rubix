#include "camera.h"

Camera::Camera(const QVector3D &eye, const QVector3D &center, const QVector3D &up)
{
    view.lookAt(eye, center, up);
    projection.setToIdentity();
}

void Camera::perspective(float verticalAngle, float aspectRation, float nearPlane, float farPlane)
{
    projection.setToIdentity();
    projection.perspective(verticalAngle, aspectRation, nearPlane, farPlane);
}
