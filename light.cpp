#include "light.h"

Light::Light(QVector3D position, QVector3D ambient, QVector3D diffuse, QVector3D specular)
{
    this->position = QVector4D(position, 1.0);
    this->ambient  = QVector4D(ambient , 1.0);
    this->diffuse  = QVector4D(diffuse , 1.0);
    this->specular = QVector4D(specular, 1.0);
}
