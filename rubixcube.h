#ifndef RUBIXCUBE_H
#define RUBIXCUBE_H

#include <QVector3D>
#include <QtOpenGL>
#include "cubepiece.h"

#define RUBIX_NUMBER_OF_PIECES    27
#define RUBIX_DISTANCE_OF_PEICES  0.6

class RubixCube
{
public:
    RubixCube(QGLWidget *widget);
    ~RubixCube();
    void rotate(float angle, const QVector3D &vec);
    void translate(const QVector3D &vec);
    void rotateR(float angle);
    void rotateL(float angle);
    void rotateB(float angle);
    void rotateD(float angle);
    void rotateF(float angle);
    void rotateU(float angle);
    void set_pMatrix(QMatrix4x4 pMatrix);
    void set_vMatrix(QMatrix4x4 vMatrix);
    void commitMovement();
    void drawObject();

protected:
    CubePiece* pieces[RUBIX_NUMBER_OF_PIECES];
    QList<CubePiece> frontPieces;

    QVector3D vecx;
    QVector3D vecy;
    QVector3D vecz;

};

#endif // RUBIXCUBE_H
