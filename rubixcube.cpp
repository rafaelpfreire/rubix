#include "rubixcube.h"

RubixCube::RubixCube(QGLWidget *widget, Light &light, Material &material, Camera &camera)
{
    int idx = 0;

    // Create 27 'CubePiece' Objects
    for(int w = 1; w >= -1; w--)
        for(int j = 1; j >= -1; j--)
            for(int i = -1; i <= 1; i++)
                pieces[idx++] = new CubePiece(widget, light, material, camera, RUBIX_DISTANCE_OF_PEICES, i, j, w);

    vecx = QVector3D(1,0,0);
    vecy = QVector3D(0,1,0);
    vecz = QVector3D(0,0,1);

    this->widget = widget;

    timer = NULL;
}

void RubixCube::setLight(Light &light)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->setLight(light);
}

void RubixCube::setCamera(Camera &camera)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->setCamera(camera);
}

void RubixCube::setMaterial(Material &material)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->setMaterial(material);
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

void RubixCube::drawObject()
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        pieces[i]->drawObject();
}

void RubixCube::rotateR(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxx() == 1 )
            pieces[i]->rotate(angle, vecx);
}

void RubixCube::rotateL(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxx() == -1 )
            pieces[i]->rotate(angle, vecx);
}

void RubixCube::rotateB(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxz() == -1 )
            pieces[i]->rotate(angle, vecz);
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
            pieces[i]->rotate(angle, vecz);
}

void RubixCube::rotateU(float angle)
{
    for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
        if( pieces[i]->idxy() == 1 )
            pieces[i]->rotate(angle, vecy);
}

void RubixCube::movement(const Rubix::MoveType move)
{
    uint randSeed;
    Movement mv;

    // Every new movement: create a new timer
    // and put the movement on the Stack (moveStack)
    if(timer == NULL)
    {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(timerInterrupt()));
        timer->start(RUBIX_TIMER_TIMEOUT_MS);

        mv.type = move;

        switch(mv.type) {

        case Rubix::ChangeViewRight:
        case Rubix::ChangeViewLeft:

            mv.finalAngle = 120;
            mv.speed = 480;
            moveStack.push(mv);
            break;

        case Rubix::Shuffle:

            // Seed for the qrand() function
            randSeed = QTime::currentTime().msec();
            qsrand(randSeed);

            for( int i = 0; i < 30; i++ )
            {
                // Random move
                mv.type = static_cast<Rubix::MoveType>(Rubix::Rmove + (qrand() % Rubix::umove));
                mv.finalAngle = 90;
                mv.speed = 360;

                // Put the movement on the stack
                moveStack.push(mv);
            }
            break;

        default:

            mv.finalAngle = 90;
            mv.speed = 360;
            moveStack.push(mv);
            break;

        }
    }
}

void RubixCube::commitMovement(const Rubix::MoveType move)
{
    QMatrix4x4 operation;
    QVector3D vec;

    switch(move) {

        case Rubix::Rmove:

            operation.rotate(90, vecx);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxx() == 1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::rmove:

            operation.rotate(-90, vecx);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxx() == 1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::Lmove:

            operation.rotate(90, vecx);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxx() == -1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::lmove:

            operation.rotate(-90, vecx);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxx() == -1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::Bmove:

            operation.rotate(90, vecz);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxz() == -1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::bmove:

            operation.rotate(-90, vecz);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxz() == -1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::Dmove:

            operation.rotate(90, vecy);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxy() == -1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::dmove:

            operation.rotate(-90, vecy);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxy() == -1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::Fmove:

            operation.rotate(90, vecz);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxz() == 1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::fmove:

            operation.rotate(-90, vecz);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxz() == 1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::Umove:

            operation.rotate(90, vecy);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxy() == 1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::umove:

            operation.rotate(-90, vecy);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                if( pieces[i]->idxy() == 1 )
                {
                    vec.setX(pieces[i]->idxx());
                    vec.setY(pieces[i]->idxy());
                    vec.setZ(pieces[i]->idxz());

                    vec = operation * vec;

                    pieces[i]->setidx(vec.x(), vec.y(), vec.z());
                }
            }
            break;

        case Rubix::RotateRight:

            vecx = QVector3D(1,0,0);
            vecy = QVector3D(0,1,0);
            vecz = QVector3D(0,0,1);

            operation.rotate(90, vecy);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                vec.setX(pieces[i]->idxx());
                vec.setY(pieces[i]->idxy());
                vec.setZ(pieces[i]->idxz());

                vec = operation * vec;

                pieces[i]->setidx(vec.x(), vec.y(), vec.z());
            }
            break;

        case Rubix::RotateLeft:

            vecx = QVector3D(1,0,0);
            vecy = QVector3D(0,1,0);
            vecz = QVector3D(0,0,1);

            operation.rotate(-90, vecy);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                vec.setX(pieces[i]->idxx());
                vec.setY(pieces[i]->idxy());
                vec.setZ(pieces[i]->idxz());

                vec = operation * vec;

                pieces[i]->setidx(vec.x(), vec.y(), vec.z());
            }
            break;

        case Rubix::RotateDown:

            vecx = QVector3D(1,0,0);
            vecy = QVector3D(0,1,0);
            vecz = QVector3D(0,0,1);

            operation.rotate(90, vecx);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                vec.setX(pieces[i]->idxx());
                vec.setY(pieces[i]->idxy());
                vec.setZ(pieces[i]->idxz());

                vec = operation * vec;

                pieces[i]->setidx(vec.x(), vec.y(), vec.z());
            }
            break;

        case Rubix::RotateUp:

            vecx = QVector3D(1,0,0);
            vecy = QVector3D(0,1,0);
            vecz = QVector3D(0,0,1);

            operation.rotate(-90, vecx);

            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
            {
                vec.setX(pieces[i]->idxx());
                vec.setY(pieces[i]->idxy());
                vec.setZ(pieces[i]->idxz());

                vec = operation * vec;

                pieces[i]->setidx(vec.x(), vec.y(), vec.z());
            }
            break;

        default:
            break;
    }
}

void RubixCube::timerInterrupt()
{
    Camera cam;
    static Movement mv;
    static int shuffleFlag = 0;
    static float angle_acc = 0;

    if( angle_acc == 0 ) {
        mv = moveStack.pop();
    }
    else if( angle_acc == mv.finalAngle )
    {
        angle_acc = 0;

        commitMovement(mv.type);

        // If the Stack is Empty, end w/ the interrupt. If it is not empty
        // after the movement ends that means that it is a shuffle movement.
        // So set the Shuffle Flag
        if( moveStack.isEmpty() )
        {
            delete timer;
            timer = NULL;

            if( shuffleFlag )
            {
                shuffleFlag = 0;
                emit shuffleEnd();
            }
        }
        else {
            shuffleFlag = 1;
        }

        if( this->isSolved() && (mv.type >= Rubix::Rmove) && (mv.type <= Rubix::umove) )
            emit solved();

        mv.finalAngle = 0;
        mv.speed = 0;
        mv.type = Rubix::noMove;
    }

    switch(mv.type) {

        case Rubix::Rmove:
            rotateR(mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::rmove:
            rotateR(-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::Lmove:
            rotateL(mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::lmove:
            rotateL(-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::Bmove:
            rotateB(mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::bmove:
            rotateB(-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::Dmove:
            rotateD(mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::dmove:
            rotateD(-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::Fmove:
            rotateF(mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::fmove:
            rotateF(-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::Umove:
            rotateU(mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::umove:
            rotateU(-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3);
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::ChangeViewRight:
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            cam = pieces[0]->getCamera();
            cam.rotate((mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3)/2, QVector3D(0,1,0));
            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
                pieces[i]->setCamera(cam);

            this->drawObject();

            break;

        case Rubix::ChangeViewLeft:
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            cam = pieces[0]->getCamera();
            cam.rotate((-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3)/2, QVector3D(0,1,0));
            for( int i = 0; i < RUBIX_NUMBER_OF_PIECES; i++ )
                pieces[i]->setCamera(cam);

            this->drawObject();

            break;

        case Rubix::RotateRight:
            rotate((mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3), QVector3D(0,1,0));
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::RotateLeft:
            rotate((-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3), QVector3D(0,1,0));
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::RotateDown:
            rotate((mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3), QVector3D(1,0,0));
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        case Rubix::RotateUp:
            rotate((-mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3), QVector3D(1,0,0));
            angle_acc += mv.speed*RUBIX_TIMER_TIMEOUT_MS*1e-3;
            break;

        default:
            break;
    }

    widget->updateGL();
}

bool RubixCube::isSolved()
{
    QStack<QVector3D> face_iPosition;
    QVector3D vec1, vec2;
    int axisFlag = 0;
    int value;

    // We're gonne loop over all the faces and see if in each face the pieces have
    // the same axis values on their initial positions. If the initial position axis
    // values (x, y or z) of all the pieces in the same face are equal, the cube is
    // solved
    for( int i = 0; i < 6; i++ )
    {
        // Plane Value (x = 1, x = -1, y = 1, and so on)
        // If 'i' is even, test for plane 1. Plane -1 otherwise
        if( (i % 2) == 0 ) value =  1;
        else               value = -1;

        // Loop over all the pieces and take the pieces of a given face.
        // Put the initial position of these pieces on the QStack
        for( int j = 0; j < RUBIX_NUMBER_OF_PIECES; j++ )
        {
            switch(i) {
            case 0:
            case 1: // Plane x = 1 or x = -1
                if( pieces[j]->idxx() == value )
                    face_iPosition.push(pieces[j]->initialPosition());
                break;
            case 2:
            case 3: // Plane y = 1 or y = -1
                if( pieces[j]->idxy() == value )
                    face_iPosition.push(pieces[j]->initialPosition());
                break;
            case 4:
            case 5: // Plane z = 1 or z = -1
                if( pieces[j]->idxz() == value )
                    face_iPosition.push(pieces[j]->initialPosition());
                break;
            default:
                break;
            }
        }

        // Take the first 2 initial position
        vec1 = face_iPosition.pop();
        vec2 = face_iPosition.pop();

        // Set bit 0, 1 or 2 if the axis x, y or z are equal, correspondingly
        if( vec1.x() == vec2.x() ) axisFlag |= (1 << 0);
        if( vec1.y() == vec2.y() ) axisFlag |= (1 << 1);
        if( vec1.z() == vec2.z() ) axisFlag |= (1 << 2);

        // Loop over all the QStack values (Pieces' initial position on the same face)
        while( !face_iPosition.isEmpty() )
        {
            // Take a new initial position on the QStack
            vec2 = face_iPosition.pop();

            // If the flag was set on a given axis however the new value's axis is
            // not equal to the first one, reset the this axis flag. If the axisFlag == 0
            // it means that all the flags were reseted therefore the cube is not solved
            if( axisFlag != 0 )
            {
                if( (axisFlag & 0x01) && (vec1.x() != vec2.x()) ) axisFlag &= ~(0x01);
                if( (axisFlag & 0x02) && (vec1.y() != vec2.y()) ) axisFlag &= ~(0x02);
                if( (axisFlag & 0x04) && (vec1.z() != vec2.z()) ) axisFlag &= ~(0x04);
            }
            else
            {
                return false;
            }
        }

        // Clear the stack and de Axis Flag
        face_iPosition.clear();
        axisFlag = 0;
    }

    return true;
}
