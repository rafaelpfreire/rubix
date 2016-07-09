#include "rubixcube.h"

RubixCube::RubixCube(QGLWidget *widget)
{
    int idx = 0;

    for(int w = 1; w >= -1; w--)
        for(int j = 1; j >= -1; j--)
            for(int i = -1; i <= 1; i++)
                pieces[idx++] = new CubePiece(widget, RUBIX_DISTANCE_OF_PEICES, i, j, w);
}

RubixCube::~RubixCube()
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ ) {
        delete pieces[i];
        pieces[i] = NULL;
    }
}

void RubixCube::rotate(float anglex, float angley, float anglez)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->rotate(anglex, angley, anglez);
}

void RubixCube::translate(float x, float y, float z)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->translate(x, y, z);
}

void RubixCube::translate(QVector3D &vector)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->translate(vector);
}

void RubixCube::set_pMatrix(QMatrix4x4 pMatrix)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->set_pMatrix(pMatrix);
}

void RubixCube::set_vMatrix(QMatrix4x4 vMatrix)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->set_vMatrix(vMatrix);
}

void RubixCube::drawObject()
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->drawObject();
}
