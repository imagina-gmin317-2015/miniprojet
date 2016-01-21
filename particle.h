#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <QVector2D>
#include <QVector3D>
#include <ctime>
#include <QDebug>
#include "utils.h"

const float MIN_MOVE_SPEED = 0.02f;
const float MAX_MOVE_SPEED = 0.03f;

const float MIN_LIFETIME = 1.0f;
const float MAX_LIFETIME = 2.0f;

class Particle
{
private:
    float _moveSpeed;
    float _lifetime;
    int _angle;
    QVector3D _position;
    QVector2D _direction;

public:
    Particle();
    Particle(QVector3D pos);
    void update();

    QVector3D getPosition();
    float getlifetime();
};

#endif // PARTICLE_H
