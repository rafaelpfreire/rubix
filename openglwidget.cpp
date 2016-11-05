#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QStatusBar *parent)
{
    QMatrix4x4 operation;
    QVector3D position;

    setFocusPolicy(Qt::StrongFocus);

    // Light Parameters
    operation.rotate(-15 , 1, 0, 0);
    position = operation * QVector3D(0, 0, 15.0);
    light = new Light(position, QVector3D(1,1,1), QVector3D(1,1,1), QVector3D(1,1,1));

    // Material Parameters
    material = new Material(100.0, QVector3D(.3,.3,.3), QVector3D(1,1,1), QVector3D(1,1,1));

    // Camera Parameters
    operation.setToIdentity();
    operation.rotate( 30, 0, 1, 0);
    operation.rotate(-30, 1, 0, 0);
    position = operation * QVector3D(0, 0, 4);
    QVector3D upDirection = operation * QVector3D(0, 1, 0);
    camera = new Camera(position, QVector3D(0,0,0), upDirection);

    // Rubix Cube and Stop Watch
    rubix = new RubixCube(this, *light, *material, *camera);
    stopWatch = new StopWatch(parent);

    connect(rubix, SIGNAL(shuffleEnd()), stopWatch, SLOT(start()));
    connect(rubix, SIGNAL(shuffleEnd()), this, SLOT(shuffleEndCallback()));

    timer = new QTimer();
    timer->setInterval(500);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(killMovement()));
}

OpenGLWidget::~OpenGLWidget()
{
    if( rubix ) {
        delete rubix;
        rubix = NULL;
    }

    delete camera;
    camera = NULL;

    delete light;
    light = NULL;

    delete material;
    material = NULL;
}

void OpenGLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(50, 50, 50));
}

void OpenGLWidget::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    // Projection Matrix initialized via "perspective" function
    camera->perspective(60.0, (float) width / (float) height, 0.001, 1000);
    glViewport(0, 0, width, height);

    rubix->setCamera(*camera);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    rubix->drawObject();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    static int directionFlag = 2;

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

    default:
        break;
    }

    updateGL();
}

void OpenGLWidget::shuffleCube()
{
    if (!isPlaying)
    {
        rubix->movement(Rubix::Shuffle);
        isPlaying = true;
    }
}

void OpenGLWidget::cubeSolvedCallback()
{
    QString str = tr("Congratulations, you have resolved the cube challenge in: ") +
                  stopWatch->toString() +
                  tr(" seconds.\nTry again!");
    QMessageBox::information(this, tr("You Won! | Rubix Cube Game 3D!"), str);

    isPlaying = false;

    disconnect(rubix, SIGNAL(solved()), stopWatch, SLOT(stop()));
    disconnect(rubix, SIGNAL(solved()), this, SLOT(cubeSolvedCallback()));
}

void OpenGLWidget::shuffleEndCallback()
{
    connect(rubix, SIGNAL(solved()), stopWatch, SLOT(stop()));
    connect(rubix, SIGNAL(solved()), this, SLOT(cubeSolvedCallback()));
}

void OpenGLWidget::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragging = true;
        xant = event->localPos().x();
        yant = event->localPos().y();
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent * event){
    if (dragging)
    {
        if (event->button() == Qt::LeftButton)
        {
            dragging = false;

            static int directionFlag = 0;

            dx = xant - event->localPos().x();
            dy = yant - event->localPos().y();

            if (abs(dx) < abs(dy)) {
                if (dy > 0) rubix->movement(Rubix::RotateUp);
                else rubix->movement(Rubix::RotateDown);
            }
            else {
                if (dx > 0)
                    if( directionFlag == 1 ) {
                        directionFlag = 0;
                        rubix->movement(Rubix::ChangeViewLeft);
                    }
                    else {
                        rubix->movement(Rubix::RotateLeft);
                    }
                else
                    if( directionFlag == 0 ) {
                        directionFlag = 1;
                        rubix->movement(Rubix::ChangeViewRight);
                    }
                    else {
                        rubix->movement(Rubix::RotateRight);
                    }
            }

            xant = event->localPos().x();
            yant = event->localPos().y();
        }
    }
}

void OpenGLWidget::killMovement(){
    dragging = false;
}
