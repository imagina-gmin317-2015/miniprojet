#include "food.h"

b2Body *Food::getBody()
{
    return _body;
}

void Food::setBody(b2Body *body)
{
    _body = body;
}

Food::Food()
{
    _position = QVector3D();
    _lifetime = 5.0f;
    _color = QVector4D(0.7, 0.2, 0.2, 1.0);
    _timer = 0.0f;
}

Food::Food(QVector3D position, float lifetime, QVector4D color)
{
    _position = position;
    _lifetime = lifetime;
    _color = color;
    _timer = 0.0f;
}


QVector3D Food::position() const
{
    return _position;
}

QVector4D Food::color() const
{
    return _color;
}

float Food::lifetime() const
{
    return _lifetime;
}

EntityType Food::getEntityType()
{
    return FOOD;
}

float Food::timer() const
{
    return _timer;
}

void Food::updateTimer()
{
    _timer += 0.1f;
}
