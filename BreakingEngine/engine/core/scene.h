#ifndef SCENE_H
#define SCENE_H
#include <QObject>
#include <QVector>
#include "common.h"
#include "engine/rendering/breakingwindow.h"
class Scene : public virtual QObject
{
    Q_OBJECT
private:
    GameObjectList m_gameObjects;

public:
    explicit Scene(BreakingWindow* parent=0);
    virtual ~Scene();
    virtual void init();
    virtual void update();
    virtual void render();

    GameObjectList renderables();
    GameObjectList updatables();

    virtual BreakingWindow* parent();

    void addGameObject(GameObject* gameObject);
    void removeGameObject(GameObject* gameObject);
signals:

public slots:
};

#endif // SCENE_H
