#ifndef CUBEPIECE_H
#define CUBEPIECE_H

#include "light.h"
#include "material.h"
#include "graphicobject.h"

class CubePiece : protected GraphicObject
{
public:
    CubePiece(QGLWidget *widget, Light *light, Material *material, float distance, int x, int y, int z);
    ~CubePiece();
    void drawObject();
    void rotate(float angle, const QVector3D &vec);
    void translate(const QVector3D &vector);
    void set_pMatrix(QMatrix4x4 pMatrix) { this->pMatrix = pMatrix; }
    void set_vMatrix(QMatrix4x4 vMatrix) { this->vMatrix = vMatrix; }
    QVector3D initialPosition() { return QVector3D(i_idxx, i_idxy, i_idxz); }
    QVector3D currentPosition() { return QVector3D(m_idxx, m_idxy, m_idxz); }
    int  idxx() { return this->m_idxx; }
    int  idxy() { return this->m_idxy; }
    int  idxz() { return this->m_idxz; }
    void setidx(int idxx, int idxy, int idxz);

protected:
    Light *light;
    Material *material;

    int m_idxx, m_idxy, m_idxz;
    int i_idxx, i_idxy, i_idxz;

    QVector4D rFaceVertices[4];
    QVector4D lFaceVertices[4];
    QVector4D uFaceVertices[4];
    QVector4D dFaceVertices[4];
    QVector4D fFaceVertices[4];
    QVector4D bFaceVertices[4];
};

#endif // CUBEPIECE_H
