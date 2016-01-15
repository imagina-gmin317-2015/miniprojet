#ifndef UPDATABLE_H
#define UPDATABLE_H

#include "component.h"

class Updatable : public virtual Component
{
public:
    Updatable(GameObject* parent=0);
    virtual ~Updatable();

    virtual void update() = 0;

};

#endif // UPDATABLE_H
