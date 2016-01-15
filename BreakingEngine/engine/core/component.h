#ifndef CORE_COMPONENT_H
#define CORE_COMPONENT_H

#include <QtCore>
#include <QObject>
#include "engine/core/common.h"

class Component : public virtual QObject {
  Q_OBJECT
public:
  explicit Component(GameObject* parent=0);
  virtual ~Component();
  GameObject* gameObject();
  Transform* transform();
public slots:
signals:
};

#endif
