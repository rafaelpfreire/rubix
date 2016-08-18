#include "light.h"

Light::Light()
{
    QMatrix4x4 operation;
    operation.rotate(-15 , 1, 0, 0);
    operation.rotate(  0 , 0, 1, 0);
    operation.rotate(  0 , 0, 0, 1);

    int distance = 15.0;
    position = operation * QVector4D(0, 0, distance, 1.0);

    ambient = QVector4D(1, 1, 1, 1.0);
    diffuse = QVector4D(1, 1, 1, 1.0);
    specular = QVector4D(1, 1, 1, 1.0);
}
