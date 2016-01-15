/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */

#include "terrain.h"
#include "commonstruct.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <math.h>
#include "breakingnoise.h"

static const char *vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";

/**
 * @brief Terrain::Terrain, constructeur de la classe Terrain, ouvrant l'image passée en paramètre.
 * @param heightmap, chemin de la heightmap permettant de générer le terrain
 * @param _app, QGuiApplication permettant la fermeture de celle-ci
 */
Terrain::Terrain(QString strHeightmap, QString strColor, QGuiApplication* _app) : m_program(0), m_frame(0), indexBuf(QOpenGLBuffer::IndexBuffer), position(0,0,-20), direction_vue_h(3.14f), direction_vue_v(0), wireframe(false)
{
    app = _app;
    heightmapPath = strHeightmap;
    heightmapColorPath = strColor;

    openImage(heightmapPath);
}

/**
 * @brief Terrain::~Terrain, destructeur de la classe Terrain.
 */
Terrain::~Terrain(){
    delete hauteur;
    delete vertices;
    delete indices;

    arrayBuf.destroy();
    indexBuf.destroy();
}

GLuint Terrain::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

/**
 * @brief Terrain::initialize, initialise les données OpenGL et variables/buffers de la classe terrain.
 * Créé également la structure du terrain.
 */
void Terrain::initialize()
{
    //Chargement du shader
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    //Linking du shader
    m_program->link();
    //Récupération des accès aux attributs et uniforms
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    // Initialisation opengl
    glEnable(GL_DEPTH_TEST);    // Active le Z-Buffer
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat direction[] = { -1.0f, -1.0f, -1.0f, 0.0f };

    GLfloat Light0Pos[4] = {0.0f, 0.0f, 20.0f, 1.0f};

    GLfloat Light0Amb[4] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat Light0Dif[4] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat Light0Spec[4]= {0.1f, 0.1f, 0.1f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, direction);
    // Fixe les paramètres de couleur de la lumière 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Spec);
    // Fixe la position de la lumière 0
    glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    souris_active = false;
    //Création des buffers VBO
    arrayBuf.create();
    indexBuf.create();
    //Création du terrain
    createTerrain();
}

/**
 * @brief Terrain::openImage, permet d'ouvrir l'image dont le chemin est passé en paramètre et de récupérer la valeur de ces pixels.
 * @param str, chemin de l'image
 */
void Terrain::openImage(QString str){
    if(!heightmap.load(str)){
        qDebug() << "Impossible de charger l'image.";
        terrain_height = 256;
        terrain_width = 256;
        Rebuild();
    }

    if(heightmap.depth() != 0){

        terrain_width = heightmap.width();
        terrain_height = heightmap.height();

        hauteur = new int[terrain_width * terrain_height];

        int index = 0;
        for(int i = 0  ; i < terrain_height ; i++){
            for(int j = 0 ; j < terrain_width ; j++){
                QRgb pixel = heightmap.pixel(i,j);
                hauteur[index++] = qGray(qRed(pixel), qGreen(pixel), qBlue(pixel));
            }
        }
    }
}

/**
 * @brief Terrain::createTerrain, créé la structure du terrain en remplissant le tableau de vertices et le tableau d'indices en fonction de la taille de la heightmap.
 * Alloue églement la taille nécessaire dans les buffers correspondant à ceux-ci.
 */
void Terrain::createTerrain(){
    q = new Quadtree(vertices, delta, resolution, terrain_width);

    if(heightmap.depth() == 0)
        return;

    QImage heightmapColor;
    if(!heightmapColor.load(heightmapColorPath)){
        qDebug() << "Impossible de charger l'image en couleur.";
        close();
    }

    //VERTICES /////////////////////////////////////////////////////////////////
    vertices = new VertexData[terrain_width * terrain_height];

    // espace entre les vertices du terrain
    float gap = 0.5f;
    // position de départ
    float posX = -(gap * terrain_width/2.f);
    float posY = -10.f;
    float posZ = -(gap * terrain_height/2.f);

    int index = 0;
    // delta de décalage de coordonées de texture
    float x = 1.f / terrain_width;
    float y = 1.f / terrain_height;

    // Génération des coordonnées (et couleur) des vertices
    for(int i = 0 ; i < terrain_height ; i++){
        for(int j = 0 ; j < terrain_width ; j++){
            QRgb pixel = heightmapColor.pixel(i,j);

            vertices[index].position = QVector3D(posX + gap * j, posY + (hauteur[index] / 20.f), posZ + gap * i);
            vertices[index].color = QVector3D(qRed(pixel) / 255.f,qGreen(pixel) / 255.f,qBlue(pixel) / 255.f);

            vertices[index].texCoord = QVector2D(x * j, y * i);
            index++;
        }
    }

    index = 0;

    // Génération des normales des vertices
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

    // Réactualisation du QuadTree
    q->Reset(vertices);
    q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
    voxels = q->getVoxels();

    if(!bBuildDecimateTerrain){
        // Haute résolution
        BuildHighResTerrain();
    }else{
        // Version décimée
        BuildStaticQuatree();
    }

    // Destruction pointeur hauteur
    delete hauteur;
}

/**
 * @brief Terrain::displayTerrain, affiche le terrain avec une structure VBO, en se servant des buffers construit précédemment.
 */
void Terrain::displayTerrain(){

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

    // Offset for color
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

/**
 * @brief Terrain::render, fonction de rendu OpenGL.
 */
void Terrain::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Réserve le shader
    m_program->bind();

    // Initialisation de la caméra
    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 500.0f);
    matrix.lookAt(position, position+direction, up);

    // Transfert de la matrice projectionViewMatrix
    m_program->setUniformValue(m_matrixUniform, matrix);

    // Affiche le terrain
    if(heightmap.depth() != 0){
        displayTerrain();
    }

    // Affiche le quadTree
    if(bDisplayQuadtree)
        q->DisplayQuadtree();

    // Libère le shader
    m_program->release();

    // Incrémente le nombre de frame
    ++m_frame;
}

/**
 * @brief Terrain::keyPressEvent, permet d'écouter les entrées clavier et de les traiter.
 * Peut changer la position de la caméra, passer en mode wireframe ou fermer l'application.
 * @param event, entrées clavier
 */
void Terrain::keyPressEvent( QKeyEvent * event )
{
    float speed = 1.f;
    if(event->key() == Qt::Key_V){
        // Affiche ou cache le QuadTree
        bDisplayQuadtree = ! bDisplayQuadtree;
    }else if(event->key() == Qt::Key_B){
        // Change entre mode haute qualité et décimée
        bBuildDecimateTerrain = ! bBuildDecimateTerrain;
        if(!bBuildDecimateTerrain){
            BuildHighResTerrain();
        }else{
            BuildStaticQuatree();
        }
    }else if(event->key() == Qt::Key_Z){
        // Déplacement vertical
        position += direction * speed;
        UpdateQuadtree();

    }else if(event->key() == Qt::Key_S){
        // Déplacement vertical
        position -= direction * speed;
        UpdateQuadtree();

    }else if(event->key() == Qt::Key_Q){
        // Déplacement vertical
        position -= right * speed;
        UpdateQuadtree();

    }else if(event->key() == Qt::Key_D){
        // Déplacement vertical
        position += right * speed;
        UpdateQuadtree();

    }else if(event->key() == Qt::Key_Up){
        // Déplacement vertical
        position.setY(position.y() + speed);
        UpdateQuadtree();

    }else if(event->key() == Qt::Key_Down){
        // Déplacement vertical
        position.setY(position.y() - speed);
        UpdateQuadtree();

    }else if(event->key() == Qt::Key_W){
        // Change entre mode wireframe et mode fill
        wireframe = !wireframe;
        if(wireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }else if(event->key() == Qt::Key_Escape){
        // Quitte le programme
        app->quit();
    }else if(event->key() == Qt::Key_Minus){
        resolution --;
        // Réduit la résolution du terrain
        if(resolution < 7){
            resolution = 7;
        }
        UpdateResolution();
    }else if(event->key() == Qt::Key_Plus){
        resolution ++;
        // Augmente la résolution du terrain
        UpdateResolution();
    }else if(event->key() == Qt::Key_0){
        // Affiche les statistiques du QuadTree
        q->ShowVoxelStat();
    }
    else if(event->key() == Qt::Key_I) {
        // Décalage du terrain
        decalX+=0.1f;
        Rebuild();
    }
    else if(event->key() == Qt::Key_K) {
        // Décalage du terrain
        decalX-=0.1f;
        Rebuild();
    }
    else if(event->key() == Qt::Key_L) {
        // Décalage du terrain
        decalY+=0.1f;
        Rebuild();
    }
    else if(event->key() == Qt::Key_J) {
        // Décalage du terrain
        decalY-=0.1f;
        Rebuild();
    }
}

/**
 * @brief Terrain::UpdateQuadtree Réactualise le QuadTree en fonction du terrain
 */
void Terrain::UpdateQuadtree(){
    int x = terrain_width / 2;
    int y = terrain_height / 2;
    int index = y * terrain_width + x;

    QVector3D center = vertices[index].position;
    float distance = sqrt(pow(position.x() - center.x(),2) + pow(position.y() - center.y(),2) + pow(position.z() - center.z(),2));

    q->setDelta((distance * 0.1) / 10);
    q->Reset(vertices);
    q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
    voxels = q->getVoxels();

    if(bBuildDecimateTerrain){
        BuildStaticQuatree();
    }
}

/**
 * @brief Terrain::UpdateResolution Augmente la taille des patchs du QuadTree
 */
void Terrain::UpdateResolution()
{
    q->setResolution(resolution);
    q->Reset(vertices);
    q->QuadtreeStatic(new Voxel(QVector3D(0,-5,0), terrain_width/2), 0, 0);
    voxels.clear();
    voxels = q->getVoxels();

    if(bBuildDecimateTerrain){
        BuildStaticQuatree();
    }
}

/**
 * @brief Terrain::Rebuild construit un terrain en bruit de Perlin et exporte les informations dans deux images
 */
void Terrain::Rebuild()
{
    // Génère un bruit de Perlin en fonction des positions
    utils::NoiseMap map = PerlinNoise(-2.5f+decalX,-2.5+decalY,5.0f,5.0f,terrain_width,
                                      terrain_height,octave,
                                      frequency,persistence);

    // Génère et transfère le résultat du bruit en niveau de gris dans l'image du terrain
    WriteImage(getHeightmapPath().toStdString(),RenderGrayScaleImage(map));
    if(gradiants.size() == 0) {
        // Génère et transfère le résultat du bruit dans une image couleur avec les gradients par défaut
        WriteImage(getHeightmapColorPath().toStdString(),RenderImage(map,lightEnabled));
    }
    else {
        // Génère et transfère le résultat du bruit dans une image couleur avec les gradients stockés
        WriteImage(getHeightmapColorPath().toStdString(),RenderImage(map,lightEnabled,gradiants));
    }

    UpdateNoise();
}

/**
 * @brief Terrain::UpdateNoise, recrée le terrain en fonction de l'image de bruit de Perlin.
 */
void Terrain::UpdateNoise()
{
    //Reload image
    openImage(heightmapPath);
    //Recreate Terrain
    createTerrain();
}

/**
 * @brief Terrain::mousePressEvent, permet d'écouter l'action sur un clic souris.
 * Défini si l'utilisateur a enfoncé le clic gauche.
 * Désactive le curseur.
 * @param event, événement de souris
 */
void Terrain::mousePressEvent( QMouseEvent * event )
{
    if(event->type() == QEvent::MouseButtonPress){
        if(event->buttons() == Qt::LeftButton){
            setCursor(Qt::BlankCursor);
            souris_active = true;
        }
    }
}

/**
 * @brief Terrain::mouseReleaseEvent, permet d'écouter l'action sur un clic souris.
 * Défini si l'utilisateur a relâché le clic gauche.
 * Réactive le curseur.
 * @param event, événement de souris
 */
void Terrain::mouseReleaseEvent( QMouseEvent * event )
{
    if(event->type() == QEvent::MouseButtonRelease){
        setCursor(Qt::BitmapCursor);
        souris_active = false;
    }

}

/**
 * @brief Terrain::mouseMoveEvent, permet d'écouter les déplacements de la souris
 * @param event, événement de souris
 */
void Terrain::mouseMoveEvent(QMouseEvent* event){

    float mouseSpeed = 0.005f;

    if(souris_active && event->type() == QEvent::MouseMove){
        float xm, ym;

        xm = (float)event->x()/width()  - 0.5f;
        ym = (float)event->y()/height() - 0.5f;

        if( xm < -0.25f )
        {
            xm = 0.25f;
        }
        else if( xm > 0.25f )
        {
            xm = -0.25f;
        }

        if( ym < -0.25f )
        {
            ym = -0.25f;
        }
        else if( ym > 0.25f )
        {
            ym = 0.25f;
        }

        direction_vue_h += mouseSpeed * float(width()/2 - event->x() );
        direction_vue_v += mouseSpeed * float( height()/2 - event->y() );

        direction = QVector3D(
                    cos(direction_vue_v) * sin(direction_vue_h),
                    sin(direction_vue_v),
                    cos(direction_vue_v) * cos(direction_vue_h)
                    );

        right = QVector3D(
                    sin(direction_vue_h - 3.14f/2.0f),
                    0,
                    cos(direction_vue_h - 3.14f/2.0f)
                    );

        up = QVector3D::crossProduct(right, direction);

        QCursor::setPos(width()/2 + QWindow::x(), height()/2 + QWindow::y());
    }
}

void Terrain::BuildStaticQuatree()
{

    indices = new GLushort[voxels.size()*6];
    int index = 0;

    for(unsigned int v = 0 ; v < voxels.size() ; v++){
        int offsetX = voxels[v]->getOffsetX();
        int offsetY = voxels[v]->getOffsetY();
        int length = voxels[v]->getLength();

        if(length == 7){
            length *= 2;
            length++;
        }else{
            length *= 2;
        }

        int lengthX = length;
        int lengthY = length;

        if(offsetX + length >= terrain_width && offsetY + length >= terrain_height){
            lengthX--;
            lengthY--;
        }
        else if(offsetX + length >= terrain_width){
            lengthX--;
        }
        else if(offsetY + length >= terrain_height){
            lengthY--;
        }

        indices[index++] = (GLushort)(offsetY * terrain_width + offsetX);
        indices[index++] = (GLushort)(offsetY * terrain_width + offsetX + lengthX);
        indices[index++] = (GLushort)((offsetY + lengthY) * terrain_width + offsetX + lengthX);

        indices[index++] = (GLushort)(offsetY * terrain_width + offsetX);
        indices[index++] = (GLushort)((offsetY + lengthY) * terrain_width + offsetX + lengthX);
        indices[index++] = (GLushort)((offsetY + lengthY) * terrain_width + offsetX);
    }

    indexBuf.bind();
    //Replace index to voxels.size() * 6 if doesn't work
    indexBuf.allocate(indices, index * sizeof(GLushort));
}

void Terrain::BuildHighResTerrain()
{
    //INDICES /////////////////////////////////////////////////////////////////
    indices = new GLushort[(terrain_width-1)*(terrain_height-1)*6];

    int index = 0;

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
    indexBuf.allocate(indices, index * sizeof(GLushort));
}

