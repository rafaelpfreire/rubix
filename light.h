#ifndef LIGHT_H
#define LIGHT_H
#include <QVector4D>
#include <QMatrix4x4>

class Light
{
public:
    Light();
    Light(QVector3D position, QVector3D ambient, QVector3D diffuse, QVector3D specular);

    QVector4D position;
    QVector4D ambient;
    QVector4D diffuse;
    QVector4D specular;
};

#endif // LIGHT_H
