#include "rubixcube.h"

RubixCube::RubixCube(QGLWidget *widget)
{
    int idx = 0;

    for(int w = 1; w >= -1; w--)
        for(int j = 1; j >= -1; j--)
            for(int i = -1; i <= 1; i++)
                pieces[idx++] = new CubePiece(widget, RUBIX_DISTANCE_OF_PEICES, i, j, w);

    vecx = QVector3D(1,0,0);
    vecy = QVector3D(0,1,0);
    vecz = QVector3D(0,0,1);
}

RubixCube::~RubixCube()
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ ) {
        delete pieces[i];
        pieces[i] = NULL;
    }
}

void RubixCube::rotate(float angle, const QVector3D &vec)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->rotate(angle, vec);

    QMatrix4x4 operation;
    operation.setToIdentity();
    operation.rotate(angle, vec);

    vecx = operation * vecx;
    vecy = operation * vecy;
    vecz = operation * vecz;
}

void RubixCube::translate(const QVector3D &vec)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->translate(vec);
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

void RubixCube::rotateR(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxx() == 1 )
            pieces[i]->rotate(-angle, vecx);
}

void RubixCube::rotateL(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxx() == -1 )
            pieces[i]->rotate(-angle, vecx);
}

void RubixCube::rotateB(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxz() == -1 )
            pieces[i]->rotate(-angle, vecz);
}

void RubixCube::rotateD(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxy() == -1 )
            pieces[i]->rotate(angle, vecy);
}

void RubixCube::rotateF(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxz() == 1 )
            pieces[i]->rotate(-angle, vecz);
}

void RubixCube::rotateU(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxy() == 1 )
            pieces[i]->rotate(angle, vecy);
}

void RubixCube::commitMovement()
{
    int idx = 0;

    for(int w = 1; w >= -1; w--)
        for(int j = 1; j >= -1; j--)
            for(int i = -1; i <= 1; i++)
                pieces[idx++]->setidx(i,j,w);

    vecx = QVector3D(1,0,0);
    vecy = QVector3D(0,1,0);
    vecz = QVector3D(0,0,1);
}
