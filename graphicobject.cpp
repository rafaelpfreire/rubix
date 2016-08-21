#include "graphicobject.h"

GraphicObject::GraphicObject(QGLWidget *widget, const QString vShaderFile, const QString fShaderFile,
                             const QString offFileName)
{
    this->widget = widget;

    this->widget->makeCurrent();

    initializeOpenGLFunctions();

    glViewport(0, 0, 640.0, 480.0);

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

    // Read the OFF File and set the vertices and indices vectors
    QFile *offfile = new QFile(offFileName);
    readOFFFile(offfile);
}

GraphicObject::~GraphicObject()
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

void GraphicObject::createVBOs()
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
    sProgram->setAttributeArray("vertex", GL_FLOAT, 0, 4, 0);
    vboVertices->allocate( vertices, vnum * sizeof(QVector4D) );

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

void GraphicObject::destroyVBOs()
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

void GraphicObject::readOFFFile(QFile *file)
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
        vertices = new QVector4D[vnum];

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

            QVector4D point(x, y, z, 1.0);
            vertices[i] =  point;
        }

        faces_location = text.pos();

        QVector3D temp;
        QVector3D midPoint = (min + max) * 0.5;
        float invdiag = 1 / (max - min).length();

        for( unsigned int i = 0; i < vnum; i++ ) {
            temp = vertices[i].toVector3D();
            temp = (temp - midPoint) * invdiag;
            vertices[i] = QVector4D(temp.x(), temp.y(), temp.z(), 1.0);
        }

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

    file->close();
}

void GraphicObject::calcVerticesNormal()
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

        u = vertices[b].toVector3D() - vertices[a].toVector3D();
        v = vertices[c].toVector3D() - vertices[b].toVector3D();

        prod = QVector3D::normal(u,v);

        normals[a] += prod;
        normals[b] += prod;
        normals[c] += prod;
    }

    for( int i = 0; i < (int)vnum; i++ )
        normals[i].normalize();
}

void GraphicObject::drawObject()
{
    this->widget->makeCurrent();
    glDrawElements( GL_TRIANGLES, inum, GL_UNSIGNED_INT, 0 );
}
