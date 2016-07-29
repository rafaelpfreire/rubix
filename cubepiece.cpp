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

    m_idxx = x;
    m_idxy = y;
    m_idxz = z;
    sProgram->setUniformValue("idx", QVector3D(m_idxx,m_idxy,m_idxz));

    // Read the OFF File and set the vertices and indices vectors
    QString pieceFileName = ":/off/rsrc/cubev2.off";
    QFile *offfile = new QFile(pieceFileName);
    readOFFFile(offfile);

    float max = 0;
    for( int i = 0; i < (int)vnum; i++ )
        if( vertices[i].x() > max )
            max = vertices[i].x();

    int ix = 0, mix = 0;
    int iy = 0, miy = 0;
    int iz = 0, miz = 0;
    for( int i = 0; i < (int)vnum; i++ ) {
        if( vertices[i].x() >= 0.99*max )
            rFaceVertices[ix++] = vertices[i];
        else if( vertices[i].x() <= -0.99*max )
            lFaceVertices[mix++] = vertices[i];
        else if( vertices[i].y() >=  0.99*max )
            uFaceVertices[iy++] = vertices[i];
        else if( vertices[i].y() <= -0.99*max )
            dFaceVertices[miy++] = vertices[i];
        else if( vertices[i].z() >=  0.99*max )
            fFaceVertices[iz++] = vertices[i];
        else if( vertices[i].z() <= -0.99*max )
            bFaceVertices[miz++] = vertices[i];
    }

    // Translate the vertices to the right position
    QMatrix4x4 operation;
    operation.setToIdentity();
    operation.translate(distance * QVector3D(m_idxx, m_idxy, m_idxz));

    for( int i = 0; i < (int)vnum; i++ )
        vertices[i] = operation * vertices[i];

    for( int i = 0; i < 4; i++ ) {
        rFaceVertices[i] = operation * rFaceVertices[i];
        lFaceVertices[i] = operation * lFaceVertices[i];
        uFaceVertices[i] = operation * uFaceVertices[i];
        dFaceVertices[i] = operation * dFaceVertices[i];
        fFaceVertices[i] = operation * fFaceVertices[i];
        bFaceVertices[i] = operation * bFaceVertices[i];
    }

    sProgram->setUniformValueArray("rFace", rFaceVertices, 4);
    sProgram->setUniformValueArray("lFace", lFaceVertices, 4);
    sProgram->setUniformValueArray("uFace", uFaceVertices, 4);
    sProgram->setUniformValueArray("dFace", dFaceVertices, 4);
    sProgram->setUniformValueArray("fFace", fFaceVertices, 4);
    sProgram->setUniformValueArray("bFace", bFaceVertices, 4);

    calcVerticesNormal();
    createVBOs();
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

    sProgram->bind();
    sProgram->setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);

    vao->bind();

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

void CubePiece::rotate(float angle, const QVector3D &vec)
{
    QMatrix4x4 operation;
    operation.rotate(angle, vec);

    for( int i = 0; i < (int)vnum; i++ )
        vertices[i] = operation * vertices[i];

    for( int i = 0; i < 4; i++ ) {
        rFaceVertices[i] = operation * rFaceVertices[i];
        lFaceVertices[i] = operation * lFaceVertices[i];
        uFaceVertices[i] = operation * uFaceVertices[i];
        dFaceVertices[i] = operation * dFaceVertices[i];
        fFaceVertices[i] = operation * fFaceVertices[i];
        bFaceVertices[i] = operation * bFaceVertices[i];
    }

    sProgram->bind();
    sProgram->setUniformValueArray("rFace", rFaceVertices, 4);
    sProgram->setUniformValueArray("lFace", lFaceVertices, 4);
    sProgram->setUniformValueArray("uFace", uFaceVertices, 4);
    sProgram->setUniformValueArray("dFace", dFaceVertices, 4);
    sProgram->setUniformValueArray("fFace", fFaceVertices, 4);
    sProgram->setUniformValueArray("bFace", bFaceVertices, 4);

    calcVerticesNormal();

    vboVertices->bind();
    vboVertices->allocate( vertices, vnum * sizeof(QVector4D) );

    vboNormals->bind();
    vboNormals->allocate( normals, vnum * sizeof(QVector3D) );
}

void CubePiece::translate(const QVector3D &vec)
{
    QMatrix4x4 operation;

    operation.setToIdentity();
    operation.translate(vec);

    for( int i = 0; i < (int)vnum; i++ )
        vertices[i] = operation * vertices[i];

    for( int i = 0; i < 4; i++ ) {
        rFaceVertices[i] = operation * rFaceVertices[i];
        lFaceVertices[i] = operation * lFaceVertices[i];
        uFaceVertices[i] = operation * uFaceVertices[i];
        dFaceVertices[i] = operation * dFaceVertices[i];
        fFaceVertices[i] = operation * fFaceVertices[i];
        bFaceVertices[i] = operation * bFaceVertices[i];
    }

    sProgram->bind();
    sProgram->setUniformValueArray("rFace", rFaceVertices, 4);
    sProgram->setUniformValueArray("lFace", lFaceVertices, 4);
    sProgram->setUniformValueArray("uFace", uFaceVertices, 4);
    sProgram->setUniformValueArray("dFace", dFaceVertices, 4);
    sProgram->setUniformValueArray("fFace", fFaceVertices, 4);
    sProgram->setUniformValueArray("bFace", bFaceVertices, 4);

    calcVerticesNormal();

    vboVertices->bind();
    vboVertices->allocate( vertices, vnum * sizeof(QVector4D) );

    vboNormals->bind();
    vboNormals->allocate( normals, vnum * sizeof(QVector3D) );
}

int CubePiece::setidx(int idxx, int idxy, int idxz) {
    this->m_idxx = idxx;
    this->m_idxx = idxy;
    this->m_idxx = idxz;
}
