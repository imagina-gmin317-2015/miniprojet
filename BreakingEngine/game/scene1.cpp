#include "scene1.h"
#include "engine/rendering/breakingwindow.h"
#include "engine/core/scene.h"
#include "engine/rendering/renderer.h"
#include "engine/core/gameobject.h"
#include "engine/core/transform.h"
#include "engine/core/updatable.h"
#include <QQuaternion>
#include "engine/core/time.h"
#include "engine/rendering/mesh.h"
#include "engine/rendering/meshrenderer.h"
#include "engine/rendering/terrain2.h"
#include "engine/core/camera.h"
#include "engine/opengl/bspmanager.h"

class CubeRenderer : public virtual Renderer {

    GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
      {-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
    GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
      {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
      {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
    GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */
public:
    void init() {
        setObjectName("rendering/CubeRenderer");
        /* Setup cube vertex data. */
         v[0][0] = v[1][0] = v[2][0] = v[3][0] = -.5;
         v[4][0] = v[5][0] = v[6][0] = v[7][0] = .5;
         v[0][1] = v[1][1] = v[4][1] = v[5][1] = -.5;
         v[2][1] = v[3][1] = v[6][1] = v[7][1] = .5;
         v[0][2] = v[3][2] = v[4][2] = v[7][2] = .5;
         v[1][2] = v[2][2] = v[5][2] = v[6][2] = -.5;
    }

    virtual void renderContent() {
        int i;
        for (i = 0; i < 6; i++) {
            glBegin(GL_QUADS);
            glNormal3fv(&n[i][0]);
            glVertex3fv(&v[faces[i][0]][0]);
            glVertex3fv(&v[faces[i][1]][0]);
            glVertex3fv(&v[faces[i][2]][0]);
            glVertex3fv(&v[faces[i][3]][0]);
            glEnd();
        }
    }
};


class Triangle : public virtual Renderer {
private:


    virtual void renderContent() {
        glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0.707f,0);
            glVertex3f(-0.5f,-0.5f,0);
            glVertex3f(0.5f,-0.5f,0);
        }
        glEnd();
    }
};

class LandMark : public virtual Renderer {

public:
    int size = 1;
    virtual void init() {}
    virtual void renderContent() {
        glColor3f(1.0f,0.0f,0.0f);
        glBegin(GL_LINES);
        {
            glVertex3f(0,0,0);
            glVertex3f(size*transform()->scaling().x(),0,0);
        }
        glEnd();
        glColor3f(0.0f,1.0f,0.0f);
        glBegin(GL_LINES);
        {
            glVertex3f(0,0,0);
            glVertex3f(0,size*transform()->scaling().y(),0);
        }

        glColor3f(0.0f,0.0f,1.0f);
        glBegin(GL_LINES);
        {
            glVertex3f(0,0,0);
            glVertex3f(0,0,size*transform()->scaling().z());
        }
        glColor3f(1.0f,1.0f,1.0f);
    }
};
//template LandMark* GameObject::addComponent();
//template CubeRenderer* GameObject::addComponent2<>();
//template Triangle* GameObject::addComponent();

#include "engine/io/input.h"

///Script Example
class AnimatorScript : public virtual Updatable {

    virtual void update() {
        //gameObject()->getComponent<Camera>();
        if(Input::getKeyDown(Qt::Key_S) || Input::getKey(Qt::Key_S))
            Camera::main_camera->transform()->Translate(QVector3D(0,0,-1));

        if(Input::getKeyDown(Qt::Key_Z) || Input::getKey(Qt::Key_Z))
            Camera::main_camera->transform()->Translate(QVector3D(0,0,1));

        if(Input::getKeyDown(Qt::Key_Q) || Input::getKey(Qt::Key_Q))
            Camera::main_camera->transform()->Translate(QVector3D(1,0,0));

        if(Input::getKeyDown(Qt::Key_D) || Input::getKey(Qt::Key_D))
            Camera::main_camera->transform()->Translate(QVector3D(-1,0,0));

        if(Input::getKeyDown(Qt::Key_R) || Input::getKey(Qt::Key_R))
            Camera::main_camera->transform()->Translate(QVector3D(0,1,0));

        if(Input::getKeyDown(Qt::Key_F) || Input::getKey(Qt::Key_F))
            Camera::main_camera->transform()->Translate(QVector3D(0,-1,0));

        if(Input::getKeyDown(Qt::Key_E) || Input::getKey(Qt::Key_E))
           Camera::main_camera->pan(1);
       if (Input::getKeyDown(Qt::Key_A) || Input::getKey(Qt::Key_A))
           Camera::main_camera->pan(-1);

       if(Input::getKeyDown(Qt::Key_Up) || Input::getKey(Qt::Key_Up))
            Camera::main_camera->tilt(-1);
       if(Input::getKeyDown(Qt::Key_Down) || Input::getKey(Qt::Key_Down))
           Camera::main_camera->tilt(1);

       if(Input::getKeyDown(Qt::Key_Left) || Input::getKey(Qt::Key_Left))
           Camera::main_camera->pan(1,Camera::main_camera->transform()->forward());

       if(Input::getKeyDown(Qt::Key_Right) || Input::getKey(Qt::Key_Right))
           Camera::main_camera->pan(-1,Camera::main_camera->transform()->forward());


    }

};

class MouseMovement : public virtual Updatable {
public:
    QVector2D oldMousePosition = QVector2D(-999,-1);

    virtual void update() {
//        if(Input::getMouse(Qt::LeftButton) || Input::getMouseDown(Qt::LeftButton) || Input::getMouseUp(Qt::LeftButton))
//            qDebug() << Input::mousePosition();
        if(Input::getMouseUp(Qt::LeftButton) || !Input::getMouseDown(Qt::LeftButton)) {
            oldMousePosition = QVector2D(-999,-999);
            return;
        }
        QVector2D mouse = Input::mousePosition();

        if(oldMousePosition.x() != -999 && oldMousePosition.y() != -999) {

            QVector2D dir = mouse - oldMousePosition;

            Camera::main_camera->pan(dir.x());
            Camera::main_camera->tilt(dir.y());

        }
        oldMousePosition = Input::mousePosition();
    }
 };

class SlideAnimation : public virtual Updatable {

    virtual void update() {
        transform()->TranslateWorld(QVector3D(0.0,Time::deltaTime(),0.0));
    }
};

//template AnimatorScript* GameObject::addComponent2<>();
Scene1::Scene1()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    //Camera GameObject
    GameObject* camera = new GameObject("_Camera");
    camera->transform()->position().setZ(3.0f);
    camera->transform()->position().setX(19.0f);
    //Camera Component
    Camera* cameraComponent;// = new Camera();
    cameraComponent = camera->addComponent2<Camera>();

    //Main Camera
    Camera::main_camera = cameraComponent;
    Camera::main_camera->setAspectRatio(4.0/3.0f);
    Camera::main_camera->setProjectionType(Camera::PerspectiveProjection);
    //Add Camera Component

//    //Script example
    AnimatorScript* animator = new AnimatorScript();
//    camera->addComponent<AnimatorScript>();
    camera->addComponent(animator);
    camera->addComponent(new MouseMovement());
    //Add GameObject in the scene
    this->addGameObject(camera);

    //LandMark GameObject
    GameObject* landMark = new GameObject("_LandMark");
//    LandMark:Renderer component
    LandMark* lm = new LandMark();
    landMark->addComponent(lm);
    landMark->transform()->setScaling(QVector3D(4,4,4));
    this->addGameObject(landMark);
////    Cube GameObject
    GameObject* cube = new GameObject("Cube");
    CubeRenderer* renderer = new CubeRenderer();
    renderer->init();
    cube->addComponent(renderer);
    this->addGameObject(cube);

    GameObject* cube2 = new GameObject("Cube2");
    CubeRenderer* renderer2 = new CubeRenderer();
    renderer2->init();
    cube2->addComponent(new SlideAnimation());
    cube2->addComponent(renderer2);
    this->addGameObject(cube2);
    cube2->transform()->Translate(QVector3D(0,3,0));
    cube2->transform()->setScaling(QVector3D(2,2,2));

    GameObject* terrain = new GameObject("Terrain");
    addGameObject(terrain);


    MeshRenderer* meshRenderer = new MeshRenderer();
    BreakingShaderProgram* bsp = new BreakingShaderProgram();
    bsp->prepareShader(":/shaders/default/default.vs",":/shaders/default/default.fs");
    BSPManager::Instance()->addBreakingShaderProgram("default",bsp);
    meshRenderer->m_breaking_shader_program = bsp;

    QImage heightMap;
    heightMap.load(":/heightmap-1.png");

    QImage colorMap;
    colorMap.load(":/heightmap-1.png");
    BreakingEngine::Terrain2* compTerrain = new BreakingEngine::Terrain2(heightMap,colorMap);
    meshRenderer->mesh = compTerrain;
    terrain->addComponent(meshRenderer);

    GameObject* water = new GameObject("water");
    water->transform()->TranslateWorld(QVector3D(0,4.2,0));
    addGameObject(water);


    MeshRenderer* waterRenderer = new MeshRenderer();
    BreakingShaderProgram* waterbsp = new BreakingShaderProgram();
    waterbsp->prepareShader(":/shaders/water/water.vs",":/shaders/water/water.fs");
    BSPManager::Instance()->addBreakingShaderProgram("water",waterbsp);
    waterRenderer->m_breaking_shader_program = waterbsp;


    QImage waterImg(256,256,QImage::Format::Format_RGB32);
    for(int i=0;i<waterImg.height();++i) {
        for(int j=0;j<waterImg.width();++j) {
            if(j%2==0) {
                waterImg.setPixel(j,i,qRgb(0,0,0));
            }
            else {
                waterImg.setPixel(j,i,qRgb(2,2,2));
            }
        }
    }

    BreakingEngine::Terrain2* compWater = new BreakingEngine::Terrain2(waterImg);
    waterRenderer->mesh = compWater;
    int index = 0;
    for(int i=0;i<waterImg.height();++i) {
        for(int j=0;j<waterImg.width();++j) {
            waterRenderer->mesh->vertices[index].color =QVector3D(0.0f,0.0f,1.0f);
            index++;
        }
    }
    water->addComponent(waterRenderer);

    qDebug() << "Scene1::end of declaration";
}

