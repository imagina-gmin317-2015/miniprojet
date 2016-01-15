#include "engine/core/gameobject.h"
#include "engine/core/component.h"
#include "engine/core/transform.h"
#include "engine/core/updatable.h"
#include "engine/core/camera.h"
#include "engine/rendering/renderer.h"
#include <type_traits>

GameObject::GameObject(GameObject *parent) : QObject(parent)
{
    setObjectName("GameObject");
    _transform = new Transform(this);
}

GameObject::GameObject(QString name, GameObject *parent) : QObject(parent)
{
    setObjectName(name);
    _transform = new Transform(this);
}

bool GameObject::activate()
{
    return m_activate;
}

void GameObject::setActivate(bool activate)
{
    m_activate = activate;
}

ComponentList &GameObject::components()
{
    return _components;
}


Transform* GameObject::transform()
{
    return _transform;
}

void GameObject::addComponent(Component* component)
{
    _components.append(component);
    component->setParent(this);
}

GameObject::~GameObject()
{
    while(!_components.empty()) {
        Component* cp = _components.front();
        _components.pop_front();
        delete cp;
    }
}
template<class T>
void GameObject::getComponents(QList<T*> *v2)
{
    for(Component* comp:components()) {
        T* ptr = nullptr;
        ptr = dynamic_cast<T*>(comp);
        if(ptr!=nullptr)
            v2->append(ptr);
    }
}

template<class T>
T* GameObject::addComponent2()
{
    T* t = new T();
    addComponent((Component*)t);
    return t;
}



template<class T>
QList<T*> GameObject::getComponents()
{
    QList<T*> comps;
    for(Component* comp:components()) {
        T* ptr = nullptr;
        ptr = dynamic_cast<T*>(comp);
        if(ptr!=nullptr)
            comps.append(ptr);
    }
    return comps;
}

template<class T>
T* GameObject::getComponent()
{
    for(Component* comp:components()) {
        T* ptr = nullptr;
        ptr = dynamic_cast<T*>(comp);
        if(ptr!=nullptr)
            return ptr;
    }
    return nullptr;
}


//TEMPLATE COMPONENT EXPLICIT DECLARATIONS

template void GameObject::getComponents(QList<Updatable*>* v2);
template void GameObject::getComponents(QList<Renderer*>* v2);
template void GameObject::getComponents(QList<Camera*>* v2);

template QList<Updatable*> GameObject::getComponents();
template QList<Renderer*> GameObject::getComponents();
template QList<Camera*> GameObject::getComponents();

template Updatable* GameObject::getComponent();
template Renderer* GameObject::getComponent();
//template Camera* GameObject::getComponent();


template Camera* GameObject::addComponent2();
