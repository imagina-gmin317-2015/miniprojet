#ifndef PLAYER_H
#define PLAYER_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QVector>
#include <QDebug>
#include <QKeyEvent>
#include "entity.h"
#include "tail.h"
#include "utils.h"
#include <Box2D/Box2D.h>

static const int TAIL_LEN_MAX = 100;


class Player : public Entity
{
private:
    b2Body* _body;
    int _id;
    QVector3D _position;
    float _moveSpeed;
    float _rotationSpeed;
    QVector4D _color;
    Tail _tail;
    QVector2D _direction;
    bool _rotateLeft;
    bool _rotateRight;
    QVector<Qt::Key> _controller;
    float _angle;
    bool alive = true;

    void rotate();


public:
    Player();
    Player(int, QVector<Qt::Key>, QVector3D, QVector4D);

    EntityType getEntityType() override;

    void computeDirection();
    void rotateLeft();
    void rotateRight();
    void updateTail();


    //GETTER & SETTER
    QVector3D position() const;
    void setPosition(const QVector3D &position);
    float moveSpeed() const;
    void setMoveSpeed(float moveSpeed);
    QVector4D color() const;
    void setColor(const QVector4D &color);
    Tail *tail();
    void setTail(const Tail &tail);
    QVector2D direction() const;
    void setDirection(const QVector2D &direction);
    int getId();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    b2Body *getBody();
    void setBody(b2Body *body);
    bool getAlive() const;
    void setAlive(bool value);
};

#endif // PLAYER_H
