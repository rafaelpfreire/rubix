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
    void rotate(float anglex, float angley, float anglez);
    void rotater(float angle);
    void rotateR(float angle);
    void rotatel(float angle);
    void rotateL(float angle);
    void rotateb(float angle);
    void rotateB(float angle);
    void rotated(float angle);
    void rotateD(float angle);
    void rotatef(float angle);
    void rotateF(float angle);
    void rotateu(float angle);
    void rotateU(float angle);
    void translate(float x, float y, float z);
    void translate(QVector3D &vector);
    void set_pMatrix(QMatrix4x4 pMatrix);
    void set_vMatrix(QMatrix4x4 vMatrix);
    void drawObject();

protected:
    CubePiece* pieces[RUBIX_NUMBER_OF_PIECES];
    QList<CubePiece> frontPieces;
//    std::vector<CubePiece> midlePieces;
//    std::vector<CubePiece> backPieces;

};

#endif // RUBIXCUBE_H
