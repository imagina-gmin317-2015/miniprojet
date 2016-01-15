#include "terrain.h"
#include "engine/core/camera.h"

Terrain::Terrain(QApplication* app, QString map) : BreakingWindow(app), indexBuf(QOpenGLBuffer::IndexBuffer)
{
    wireframe = false;
    openImage(map);
}

Terrain::~Terrain()
{
    delete texture;

    delete hauteur;
    delete vertices;
    delete indices;

    arrayBuf.destroy();
    indexBuf.destroy();
}

void Terrain::initialize()
{
    m_program = new QOpenGLShaderProgram(this);

    // Compile vertex shader
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!m_program->link())
        close();

    // Bind shader pipeline for use
    if (!m_program->bind())
        close();

    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_texAttr = m_program->attributeLocation("texAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    glEnable(GL_DEPTH_TEST);    // Active le Z-Buffer
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    //Add ambient light
    GLfloat ambientColor[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add directionnal light
    GLfloat lightColor0[] = {1.f, 1.f, 1.f, 1.0f};
    GLfloat lightPos0[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //Create buffers
    arrayBuf.create();
    indexBuf.create();

    arrayBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    indexBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    //Create Terrain
    createTerrain();
}

void Terrain::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    glPushMatrix();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 500.0f);
    QVector3D up = Camera::main_camera->upVector();
    QVector3D pos = Camera::main_camera->transform()->position();
    QVector3D dir = Camera::main_camera->forwardVector();
    matrix.lookAt(pos, pos+dir, up);

    glBindTexture(GL_TEXTURE_2D, m_texture_location);

    m_program->setUniformValue(m_matrixUniform, matrix);
    //m_program->setUniformValue(m_program->uniformLocation("tex"), m_texture_location);
    //m_program->setUniformValue(m_program->uniformLocation("lightDir"), QVector3D(xDir,-1,zDir));

    if(wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if(heightmap.depth() != 0){
        displayTerrain();
    }
    glPopMatrix();

    m_program->release();

    ++m_frame;
}

bool Terrain::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

GLuint Terrain::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void Terrain::openImage(QString str)
{
    if(!heightmap.load(str))
        close();

    if(heightmap.depth() != 0){

        terrain_width = heightmap.width();
        terrain_height = heightmap.height();

        hauteur = new int[terrain_width * terrain_height];

        int index = 0;
        for(int i = 0  ; i < terrain_height ; i++){
            for(int j = 0 ; j < terrain_width ; j++){
                QRgb pixel = heightmap.pixel(i,j);
                hauteur[index++] = qRed(pixel);
            }
        }
    }
}

void Terrain::createTerrain()
{
    if(heightmap.depth() == 0)
        return;

    //VERTICES /////////////////////////////////////////////////////////////////
    delete vertices;
    vertices = new VertexData[terrain_width * terrain_height];

    float gap = 0.5f;
    float posX = -(gap * terrain_width/2.f);
    float posY = -10.f;
    float posZ = -(gap * terrain_height/2.f);

    int index = 0;
    float x = 1.f / terrain_width;
    float y = 1.f / terrain_height;
    for(int i = 0 ; i < terrain_height ; i++){
        for(int j = 0 ; j < terrain_width ; j++){
            vertices[index].position = QVector3D(posX + gap * j, posY + hauteur[index] / 20.f, posZ + gap * i);
            if(hauteur[index] < 85){
                vertices[index].color = QVector3D(0.f,0.5f,0.f);
            }else if(hauteur[index] < 170){
                vertices[index].color = QVector3D(0.33f,0.15f,0.f);
            }else{
                vertices[index].color = QVector3D(1.f,1.f,1.f);
            }

            vertices[index].texCoord = QVector2D(x * j, y * i);
            index++;
        }
    }

    index = 0;

    for(int i = 0 ; i < terrain_height ; i++){
        for(int j = 0 ; j < terrain_width ; j++){
            if(j == terrain_width - 1){
                vertices[index].normal = vertices[index-1].normal;
            }
            else if(i == terrain_height - 1){
                vertices[index].normal = vertices[index - terrain_width].normal;
            }else{
                vertices[index].normal = QVector3D::normal(vertices[index].position, vertices[index+1].position, vertices[index + terrain_width].position);
            }
            index++;
        }
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, terrain_width * terrain_height * sizeof(VertexData));

    //INDICES /////////////////////////////////////////////////////////////////
    indices = new GLushort[(terrain_width-1)*(terrain_height-1)*6];

    index = 0;

    for(int i = 0 ; i < terrain_height-1 ; i++){
        for(int j = 0 ; j < terrain_width-1 ; j++){

            indices[index++] = GLushort(i * terrain_width + j);
            indices[index++] = GLushort((i+1) * terrain_width + j);
            indices[index++] = GLushort((i+1) * terrain_width + j + 1);

            indices[index++] = GLushort(i * terrain_width + j);
            indices[index++] = GLushort((i+1) * terrain_width + j + 1);
            indices[index++] = GLushort(i * terrain_width + j + 1);
        }
    }

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, (terrain_width-1)*(terrain_height-1)*6 * sizeof(GLushort));
}

void Terrain::displayTerrain()
{
    if(arrayBuf.size() <= 0){
        return;
    }

    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_program->attributeLocation("posAttr");
    m_program->enableAttributeArray(vertexLocation);
    m_program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate color data
    int colorLocation = m_program->attributeLocation("colAttr");
    m_program->enableAttributeArray(colorLocation);
    m_program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = m_program->attributeLocation("texAttr");
    m_program->enableAttributeArray(texcoordLocation);
    m_program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2,sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector2D);

    // Tell OpenGL programmable pipeline how to locate vertex normals data
    int normalLocation = m_program->attributeLocation("normalAttr");
    m_program->enableAttributeArray(normalLocation);
    m_program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3,sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, indexBuf.size(), GL_UNSIGNED_SHORT, 0);
}

void Terrain::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_W:
            wireframe = !wireframe;
            break;
    }
}



