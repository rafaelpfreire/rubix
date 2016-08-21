#ifndef MATERIAL_H
#define MATERIAL_H
#include <QVector4D>
#include <QVector3D>

class Material
{
public:
    Material();
    Material(float shininess, QVector3D ambient, QVector3D diffuse, QVector3D specular);

    QVector4D ambient;
    QVector4D diffuse;
    QVector4D specular;
    double shininess;
};

#endif // MATERIAL_H
