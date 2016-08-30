#ifndef RUBIXCUBE_H
#define RUBIXCUBE_H

#include <QVector3D>
#include <QtOpenGL>
#include <QTimer>
#include <QQuaternion>
#include "cubepiece.h"

#define RUBIX_NUMBER_OF_PIECES    27
#define RUBIX_DISTANCE_OF_PEICES  0.6
#define RUBIX_TIMER_TIMEOUT_MS    25

namespace Rubix {
    typedef enum MoveType {
        noMove = 0,
        Rmove,
        rmove,
        Lmove,
        lmove,
        Bmove,
        bmove,
        Dmove,
        dmove,
        Fmove,
        fmove,
        Umove,
        umove,
        ChangeViewRight,
        ChangeViewLeft,
        RotateRight,
        RotateLeft,
        RotateDown,
        RotateUp,
        Shuffle
    }MoveType;
}

class RubixCube : public QObject
{
    Q_OBJECT

    typedef struct Movement {
        float speed;
        float finalAngle;
        Rubix::MoveType type;
    }Movement;

public:
    RubixCube(QGLWidget *widget, Light &light, Material &material, Camera &camera);
    ~RubixCube();
    void rotate(float angle, const QVector3D &vec);
    void translate(const QVector3D &vec);
    void movement(const Rubix::MoveType move);
    void setLight(Light &light);
    void setCamera(Camera &camera);
    void setMaterial(Material &material);
    void drawObject();
    bool isSolved();

protected:
    void rotateR(float angle);
    void rotateL(float angle);
    void rotateB(float angle);
    void rotateD(float angle);
    void rotateF(float angle);
    void rotateU(float angle);
    void commitMovement(const Rubix::MoveType move);

    CubePiece* pieces[RUBIX_NUMBER_OF_PIECES];
    QTimer* timer;
    QGLWidget* widget;
    QStack<Movement> moveStack;

    QVector3D vecx;
    QVector3D vecy;
    QVector3D vecz;

signals:
    void solved();
    void shuffleEnd();

protected slots:
    void timerInterrupt();
};

#endif // RUBIXCUBE_H
