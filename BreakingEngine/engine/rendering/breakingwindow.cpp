#include "breakingwindow.h"
#include "engine/io/input.h"
#include "engine/core/scene.h"
#include <QDebug>
#include "engine/core/camera.h"
#include "engine/opengl/bspmanager.h"
BreakingWindow* BreakingWindow::main_window = nullptr;
QApplication* BreakingWindow::app = nullptr;

BreakingWindow::BreakingWindow(QApplication* app, QWindow* parent) : OpenGLWindow(parent)
{
    setTitle("BreakingEngine");
    BreakingWindow::main_window = this;
    BreakingWindow::app = app;
}

BreakingWindow::~BreakingWindow()
{
    if(m_scene!= nullptr)
        delete m_scene;

    delete m_timer;
}

void BreakingWindow::setupScene(Scene *scene)
{
    if(m_scene != nullptr)
        delete m_scene;
    m_scene = scene;
    m_scene->setParent(this);
//    m_scene->init();
}

void BreakingWindow::initialize() {
    qDebug() <<"BreakingWindow::initialize";
    m_timer = new QTimer();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(renderNow()));
    m_timer->start(1000.0/60.0);

    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(true);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    BSPManager::Instance()->buildAll();
    m_scene->init();
    qDebug() << "BreakingWindow:: end of initialize";
}


void BreakingWindow::render()
{
   m_scene->update();
   m_scene->render();
   Input::singleton.purge();
}

void BreakingWindow::keyPressEvent(QKeyEvent * event) {
    if(event->type() == QEvent::KeyPress) {
        Input::singleton.handlePressedKeyEvent(event);
        event->accept();
    }
}

void BreakingWindow::keyReleaseEvent(QKeyEvent * event) {
    if(event->type() == QEvent::KeyRelease) {
        if(!event->isAutoRepeat()) {
        Input::singleton.handleReleasedKeyEvent(event);
        event->accept();
        }
    }
}

void BreakingWindow::mousePressEvent(QMouseEvent *event) {
    Input::singleton.handleMouseEvent(event);
    event->accept();
}

void BreakingWindow::mouseReleaseEvent(QMouseEvent *event) {
    Input::singleton.handleMouseEvent(event);
    event->accept();
}

void BreakingWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    Input::singleton.handleMouseEvent(event);
    event->accept();
}

void BreakingWindow::mouseMoveEvent(QMouseEvent * event) {
    Input::singleton.handleMouseEvent(event);
    event->accept();
}
