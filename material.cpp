#include "material.h"

Material::Material(float shininess, QVector3D ambient, QVector3D diffuse, QVector3D specular)
{
    this->ambient   = QVector4D(ambient , 1.0);
    this->diffuse   = QVector4D(diffuse , 1.0);
    this->specular  = QVector4D(specular, 1.0);
    this->shininess = shininess;
}
