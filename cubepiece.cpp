#include "cubepiece.h"

CubePiece::CubePiece(QGLWidget *widget, Light &light, Material &material, Camera &camera,
                     float distance, int x, int y, int z) :
    GraphicObject(widget, QString(":/Shaders/vshader.glsl"), QString(":/Shaders/fshader.glsl"),
                  QString(":/Cube-pieces/cubev2.off"))
{
    // Current and Initial Positions
    m_idxx = i_idxx = x;
    m_idxy = i_idxy = y;
    m_idxz = i_idxz = z;
    sProgram->setUniformValue("idx", QVector3D(i_idxx,i_idxy,i_idxz));

    // Discover the Face's Coordinates
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

    // Apply the trasformation to all the vertices
    for( int i = 0; i < (int)vnum; i++ )
        vertices[i] = operation * vertices[i];

    // Apply the transformation to all the Faces positions
    for( int i = 0; i < 4; i++ ) {
        rFaceVertices[i] = operation * rFaceVertices[i];
        lFaceVertices[i] = operation * lFaceVertices[i];
        uFaceVertices[i] = operation * uFaceVertices[i];
        dFaceVertices[i] = operation * dFaceVertices[i];
        fFaceVertices[i] = operation * fFaceVertices[i];
        bFaceVertices[i] = operation * bFaceVertices[i];
    }

    // Set the shader's values
    sProgram->setUniformValueArray("rFace", rFaceVertices, 4);
    sProgram->setUniformValueArray("lFace", lFaceVertices, 4);
    sProgram->setUniformValueArray("uFace", uFaceVertices, 4);
    sProgram->setUniformValueArray("dFace", dFaceVertices, 4);
    sProgram->setUniformValueArray("fFace", fFaceVertices, 4);
    sProgram->setUniformValueArray("bFace", bFaceVertices, 4);

    // Set light, camera and material parameters
    this->light    = light;
    this->camera   = camera;
    this->material = material;
    sProgram->setUniformValue("lightPosition", this->light.position);
    sProgram->setUniformValue("shininess", static_cast<GLfloat>(this->material.shininess));
    sProgram->setUniformValue("aProd", this->light.ambient * this->material.ambient);
    sProgram->setUniformValue("dProd", this->light.diffuse * this->material.diffuse);
    sProgram->setUniformValue("sProd", this->light.specular* this->material.specular);

    // Calculate the Normal Vectors
    calcVerticesNormal();
    createVBOs();
}

CubePiece::~CubePiece()
{
}

void CubePiece::setLight(Light &light)
{
    this->light = light;
}

void CubePiece::setCamera(Camera &camera)
{
    this->camera = camera;
}

void CubePiece::setMaterial(Material &material)
{
    this->material= material;
}

void CubePiece::drawObject()
{
    this->widget->makeCurrent();

    QMatrix4x4 mMatrix;
    mMatrix.setToIdentity();
    mMatrix.rotate(quat);

    sProgram->bind();
    sProgram->setUniformValue("mMatrix", mMatrix);
    sProgram->setUniformValue("vMatrix", camera.getView());
    sProgram->setUniformValue("pMatrix", camera.getProjection());
    sProgram->setUniformValue("nMatrix", mMatrix.normalMatrix());

    vao->bind();

    glDrawElements( GL_TRIANGLES, inum, GL_UNSIGNED_INT, 0 );
}

void CubePiece::rotate(float angle, const QVector3D &vec)
{
    quat = QQuaternion::fromAxisAndAngle(vec, angle) * quat;
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

void CubePiece::setidx(int idxx, int idxy, int idxz) {
    this->m_idxx = idxx;
    this->m_idxy = idxy;
    this->m_idxz = idxz;
}
