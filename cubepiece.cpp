#include "cubepiece.h"

CubePiece::CubePiece(QGLWidget *widget, float distance, int x, int y, int z)
{
    this->widget = widget;

    this->widget->makeCurrent();

    initializeOpenGLFunctions();

    vMatrix.setToIdentity();
    pMatrix.setToIdentity();

    glViewport(0, 0, 640.0, 480.0);

    QString vShaderFile = ":/shader/sh/vshader.glsl";
    QString fShaderFile = ":/shader/sh/fshader.glsl";

    vertexShader = new QGLShader( QGLShader::Vertex );
    if( !vertexShader->compileSourceFile(vShaderFile) )
        qWarning() << vertexShader->log();

    fragmentShader = new QGLShader( QGLShader::Fragment );
    if( !fragmentShader->compileSourceFile(fShaderFile) )
        qWarning() << fragmentShader->log();

    sProgram = new QGLShaderProgram();
    sProgram->addShader(vertexShader);
    sProgram->addShader(fragmentShader);
    sProgram->link();
    sProgram->bind();

    indices = NULL;
    vertices = NULL;
    normals = NULL;
    vboVertices = NULL;
    vboIndices  = NULL;
    vboNormals  = NULL;
    color = NULL;
    vao = NULL;
    vnum = 0;
    fnum = 0;
    inum = 0;

    position.setX(distance*x);
    position.setY(distance*y);
    position.setZ(distance*z);
    position.setW(1);

    m_idxx = x;
    m_idxy = y;
    m_idxz = z;

    sProgram->setUniformValue("idx", QVector3D(m_idxx,m_idxy,m_idxz));

    angle.setX(0);
    angle.setY(0);
    angle.setZ(0);

    QString pieceFileName = ":/off/rsrc/cubev2.off";
    QFile *offfile = new QFile(pieceFileName);
    readOFFFile(offfile);
}

CubePiece::~CubePiece()
{
    delete [] indices;
    indices = NULL;

    delete vertices;
    vertices = NULL;

    delete color;
    color = NULL;

    delete vertexShader;
    vertexShader = NULL;

    delete fragmentShader;
    fragmentShader = NULL;

    if( sProgram ) {
        sProgram->release();
        delete sProgram;
        sProgram = NULL;
    }

    if( vertexShader ) {
        delete vertexShader;
        vertexShader = NULL;
    }

    if( fragmentShader ) {
        delete fragmentShader;
        fragmentShader = NULL;
    }

    destroyVBOs();
}

void CubePiece::createVBOs()
{
    destroyVBOs();

    vao = new QOpenGLVertexArrayObject(widget);
    vao->create();
    vao->bind();

    vboVertices = new QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
    vboVertices->create();
    vboVertices->setUsagePattern( QOpenGLBuffer::StaticDraw );
    vboVertices->bind();
    sProgram->enableAttributeArray("vertex");
    sProgram->setAttributeArray("vertex", GL_FLOAT, 0, 3, 0);
    vboVertices->allocate( vertices, vnum * sizeof(QVector3D) );

    vboIndices = new QOpenGLBuffer( QOpenGLBuffer::IndexBuffer );
    vboIndices->create();
    vboIndices->setUsagePattern( QOpenGLBuffer::StaticDraw );
    vboIndices->bind();
    vboIndices->allocate( indices, inum * sizeof(unsigned int) );

    vboNormals = new QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
    vboNormals->create();
    vboNormals->setUsagePattern( QOpenGLBuffer::StaticDraw );
    vboNormals->bind();
    vboNormals->allocate( normals, vnum * sizeof(QVector3D) );
    sProgram->enableAttributeArray("normal");
    sProgram->setAttributeArray("normal", GL_FLOAT, 0, 3, 0);
}

void CubePiece::destroyVBOs()
{
    if( vboVertices ) {
        vboVertices->release();
        vboVertices->destroy();
        delete vboVertices;
        vboVertices = NULL;
    }

    if( vboIndices ) {
        vboIndices->release();
        vboIndices->destroy();
        delete vboIndices;
        vboIndices = NULL;
    }

    if( vboNormals ) {
        vboNormals->release();
        vboNormals->destroy();
        delete vboNormals;
        vboNormals = NULL;
    }

    if( vao ) {
        delete vao;
        vao = NULL;
    }
}

void CubePiece::drawObject()
{
    this->widget->makeCurrent();

    QMatrix4x4 mMatrix;

    mMatrix.setToIdentity();
    mMatrix.translate(position.toVector3D());
    mMatrix.rotate(this->angle.x(), 1, 0, 0);
    mMatrix.rotate(this->angle.y(), 0, 1, 0);
    mMatrix.rotate(this->angle.z(), 0, 0, 1);

    sProgram->bind();
    sProgram->setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);

    glDrawElements( GL_TRIANGLES, inum, GL_UNSIGNED_INT, 0 );
}

void CubePiece::readOFFFile(QFile *file)
{
    int faces_location;

    if( !file->open(QIODevice::ReadOnly) ) {
        QMessageBox::warning(widget, "Open File", "Not able to open the file!");
        return;
    }

    QTextStream text(file);
    QStringList strList;
    QString str;

    if(  text.readLine().compare("OFF") == 0 ) {

        strList = text.readLine().split(' ');

        str = strList.at(0);
        vnum = str.toInt();

        str = strList.at(1);
        fnum = str.toInt();

        delete[] vertices;
        vertices = NULL;
        vertices = new QVector3D[vnum];

        delete[] indices;
        indices = NULL;

        float minLim = std::numeric_limits<float>::min();
        float maxLim = std::numeric_limits<float>::max();
        QVector3D max(minLim, minLim, minLim);
        QVector3D min(maxLim, maxLim, maxLim);

        for( unsigned int i = 0; i < vnum; i++ ) {

            float x, y, z;

            strList = text.readLine().split(' ');

            str = strList.at(0);
            x = str.toFloat();

            str = strList.at(1);
            y = str.toFloat();

            str = strList.at(2);
            z = str.toFloat();

            max.setX( qMax<float>(max.x(),x) );
            max.setY( qMax<float>(max.y(),y) );
            max.setZ( qMax<float>(max.z(),z) );
            min.setX( qMin<float>(min.x(),x) );
            min.setY( qMin<float>(min.y(),y) );
            min.setZ( qMin<float>(min.z(),z) );

            QVector3D point(x, y, z);
            vertices[i] =  point;
        }

        faces_location = text.pos();

        QVector3D midPoint = (min + max) * 0.5;
        float invdiag = 1 / (max - min).length();

        for( unsigned int i = 0; i < vnum; i++ )
            vertices[i] = (vertices[i] - midPoint) * invdiag;

        // Discover the number of indices
        for( int i = 0; i < (int)fnum; i++ ) {
            strList = text.readLine().split(' ');

            str = strList.at(0);

            if( str.toUInt() == 3 )
                inum += 3;
            else
                inum += 6;
        }

        fnum = inum/3;
        indices = new unsigned int[inum];

        text.seek(faces_location);

        unsigned int i = 0;
        while( i < inum ) {

            strList = text.readLine().split(' ');

            str = strList.at(1);
            indices[i++] = str.toUInt();

            str = strList.at(2);
            indices[i++] = str.toUInt();

            str = strList.at(3);
            indices[i++] = str.toUInt();

            str = strList.at(0);

            if( str.toUInt() == 4 ) {

                str = strList.at(3);
                indices[i++] = str.toUInt();

                str = strList.at(4);
                indices[i++] = str.toUInt();

                str = strList.at(1);
                indices[i++] = str.toUInt();
            }
        }
    }

    calcVerticesNormal();

    file->close();

    createVBOs();
}

void CubePiece::calcVerticesNormal()
{
    QVector3D  v, u, prod;
    unsigned int a, b,c;

    delete[] normals;
    normals = NULL;
    normals = new QVector3D[vnum];

    // Inicializa com zero
    for( int i = 0; i < (int)vnum; i++ ) {
        normals[i].setX(0);
        normals[i].setY(0);
        normals[i].setZ(0);
    }

    // Acumula todas as normais para cada Vertice
    for( int i = 0; i < (int)fnum; i++ ) {
        a = indices[i*3];
        b = indices[i*3+1];
        c = indices[i*3+2];

        u = vertices[b] - vertices[a];
        v = vertices[c] - vertices[b];

        prod = QVector3D::normal(u,v);

        normals[a] += prod;
        normals[b] += prod;
        normals[c] += prod;
    }

    for( int i = 0; i < (int)vnum; i++ )
        normals[i].normalize();
}

void CubePiece::rotate(float anglex, float angley, float anglez)
{
    QMatrix4x4 operation;

    this->angle += QVector3D(anglex, angley, anglez);

    operation.rotate(anglex, 1, 0, 0);
    operation.rotate(angley, 0, 1, 0);
    operation.rotate(anglez, 0, 0, 1);
    position = operation*position;
}

void CubePiece::translate(QVector3D &vector)
{
    QMatrix4x4 operation;

    operation.setToIdentity();
    operation.translate(vector);

    position = operation*position;
}

void CubePiece::translate(float x, float y, float z)
{
    QVector3D vector(x, y, z);
    translate(vector);
}
