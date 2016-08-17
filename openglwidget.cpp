#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QStatusBar *parent)
{
    alpha = 30;
    beta = -30;
    gama = 0;
    distance = 4;

    setFocusPolicy(Qt::StrongFocus);

    rubix = new RubixCube(this);
    stopWatch = new StopWatch(parent);

    connect(rubix, SIGNAL(shuffleEnd()), stopWatch, SLOT(start()));
    connect(rubix, SIGNAL(shuffleEnd()), this, SLOT(shuffleEndCallback()));

    QMatrix4x4 vMatrix;
    QMatrix4x4 cameraTransformation;

    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(beta , 1, 0, 0);
    cameraTransformation.rotate(gama , 0, 0, 1);

    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    // eye:        Cameras position to real world
    // center:     Target Point in real world
    // up vector:  Where is the up direction
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    rubix->set_vMatrix(vMatrix);
}

OpenGLWidget::~OpenGLWidget()
{
    if( rubix ) {
        delete rubix;
        rubix = NULL;
    }
}

void OpenGLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(Qt::black));
}

void OpenGLWidget::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    // Projection Matrix initialized via "perspective" function
    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);
    glViewport(0, 0, width, height);

    rubix->set_pMatrix(this->pMatrix);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rubix->drawObject();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->buttons() & Qt::LeftButton )
        qDebug() << rubix->isSolved();
    else if( event->buttons() & Qt::RightButton )
        rubix->rotate(45, QVector3D(1,0,0));
    else if( event->buttons() & Qt::MidButton )
//        rubix->rotateU();

    updateGL();
    event->accept();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
//    int deltaX = event->x() - lastMousePosition.x();
//    int deltaY = event->y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
//        alpha -= deltaX;
//        while (alpha < 0) {
//            alpha += 360;
//        }
//        while (alpha >= 360) {
//            alpha -= 360;
//        }
//        beta -= deltaY;
//        if (beta < 0) {
//            beta += 360;
//        }
//        if (beta >= 360) {
//            beta -= 360;
//        }
        updateGL();
    }

    lastMousePosition = event->pos();
    event->accept();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            distance *= 1.1;
        } else if (delta > 0) {
            distance *= 0.9;
        }
        updateGL();
    }

    event->accept();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    static int directionFlag = 0;

    switch (event->key()) {
    case 'D':

        if( event->modifiers() == Qt::ShiftModifier )
            rubix->movement(Rubix::Rmove);
        else
            rubix->movement(Rubix::rmove);

        break;

    case 'A':

        if( event->modifiers() == Qt::ShiftModifier )
            rubix->movement(Rubix::Lmove);
        else
            rubix->movement(Rubix::lmove);

        break;

    case 'S':

        if( event->modifiers() == Qt::ShiftModifier )
            rubix->movement(Rubix::Dmove);
        else
            rubix->movement(Rubix::dmove);

        break;

    case 'W':

        if( event->modifiers() == Qt::ShiftModifier )
            rubix->movement(Rubix::Umove);
        else
            rubix->movement(Rubix::umove);

        break;

    case 'F':

        if( event->modifiers() == Qt::ShiftModifier )
            rubix->movement(Rubix::Fmove);
        else
            rubix->movement(Rubix::fmove);

        break;

    case 'Q':

        if( event->modifiers() == Qt::ShiftModifier )
            rubix->movement(Rubix::Bmove);
        else
            rubix->movement(Rubix::bmove);

        break;

    case Qt::Key_Up:
        rubix->movement(Rubix::RotateUp);
        break;

    case Qt::Key_Down:
        rubix->movement(Rubix::RotateDown);
        break;

    case Qt::Key_Right:
        // If it was rotating left, change the view first
        if( directionFlag == 0 ) {
            directionFlag = 1;
            rubix->movement(Rubix::ChangeViewRight);
        }
        else {
            rubix->movement(Rubix::RotateRight);
        }

        break;

    case Qt::Key_Left:
        // If it was rotating right, change the view first
        if( directionFlag == 1 ) {
            directionFlag = 0;
            rubix->movement(Rubix::ChangeViewLeft);
        }
        else {
            rubix->movement(Rubix::RotateLeft);
        }

        break;

    case Qt::Key_Space:
        rubix->movement(Rubix::Shuffle);

    default:
        break;
    }

    updateGL();
}

void OpenGLWidget::cubeSolvedCallback()
{
    QString str = "Congratulations, your time was: " +
                  stopWatch->toString() +
                  "\nTry again! I really believe you can reach the World Record:\n\n" +
                  "     00:04:90   Lucas Etter (USA)";
    QMessageBox::information(this, "You Win!", str);

    disconnect(rubix, SIGNAL(solved()), stopWatch, SLOT(stop()));
    disconnect(rubix, SIGNAL(solved()), this, SLOT(cubeSolvedCallback()));
}

void OpenGLWidget::shuffleEndCallback()
{
    connect(rubix, SIGNAL(solved()), stopWatch, SLOT(stop()));
    connect(rubix, SIGNAL(solved()), this, SLOT(cubeSolvedCallback()));
}
