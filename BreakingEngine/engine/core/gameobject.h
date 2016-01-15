#ifndef CORE_GAMEOBJECT_H
#define CORE_GAMEOBJECT_H

#include <QtCore>
#include <QObject>
#include <QList>
#include "engine/core/common.h"

class GameObject : public virtual QObject {
  Q_OBJECT
private:
    bool m_activate = true;
    Transform* _transform;
    QList<Component*> _components;
public:
  explicit GameObject(GameObject* parent=0);
  GameObject(QString name,GameObject* parent=0);


  bool activate();
  void setActivate(bool activate);

  Transform* transform();

  void addComponent(Component* component);

  template<class T>
  T* addComponent2();

  ComponentList& components();

  template<class T>
  void getComponents(QList<T*>* v2);
  template<class T>
  QList<T*> getComponents();
  template<class T>
  T* getComponent();


  virtual ~GameObject();
public slots:

signals:

};




#endif
