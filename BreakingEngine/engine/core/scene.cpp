#include "scene.h"
#include "component.h"
#include "gameobject.h"
#include "engine/rendering/renderer.h"
#include "engine/core/updatable.h"
#include <GL/gl.h>
#include "camera.h"
#include "engine/core/time.h"
#include "engine/rendering/breakingwindow.h"

Scene::Scene(BreakingWindow* parent) : QObject(parent)
{

}

Scene::~Scene() {
    while(!m_gameObjects.empty()) {
        GameObject* go = m_gameObjects.front();
        m_gameObjects.pop_front();
        delete go;
    }
}

void Scene::init() {
    for(GameObject* GameObject: m_gameObjects) {
        for(Renderer* renderer: GameObject->getComponents<Renderer>()) {
            renderer->init();
        }
    }
}


void Scene::update() {
    for(GameObject* gameObject: m_gameObjects) {
        QList<Updatable*> updatables;

        gameObject->getComponents<Updatable>(&updatables);

        for(Updatable* updatable: updatables) {
            updatable->update();
        }
    }
}

BreakingWindow* Scene::parent() {
    BreakingWindow* ptr = dynamic_cast<BreakingWindow*>(QObject::parent());
    return ptr;
}

void Scene::render() {
    Time::singleton.newFrame();
    glViewport(0, 0, this->parent()->width(), this->parent()->height());

    glClearColor(0.0, 0.0, 0.0, 0.0);
    //Clean flags
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(GameObject* gameObject: m_gameObjects) {
        QList<Renderer*> renderers = gameObject->getComponents<Renderer>();
        for(Renderer* renderer: renderers) {
            renderer->render();
        }
    }

}

void Scene::addGameObject(GameObject *gameObject)
{
    m_gameObjects.append(gameObject);
}

void Scene::removeGameObject(GameObject *gameObject)
{
    m_gameObjects.removeOne(gameObject);
}
