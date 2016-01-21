#ifndef FOOD_H
#define FOOD_H

#include <QVector3D>
#include <QVector4D>

#include "entity.h"
#include "Box2D/Box2D.h"


class Food : public Entity
{
private:
    QVector3D _position;
    float _lifetime;
    float _timer;
    QVector4D _color;
    b2Body *_body;

public:
    Food();
    Food(QVector3D position, float lifetime, QVector4D color);

    QVector3D position() const;

    QVector4D color() const;

    float lifetime() const;

    EntityType getEntityType() override;

    float timer() const;
    void updateTimer();
    b2Body *getBody() ;
    void setBody(b2Body *body);
};

#endif // FOOD_H
