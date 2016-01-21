#include "player.h"

Player::Player()
{
}

Player::Player(int num, QVector<Qt::Key> controller, QVector3D position, QVector4D color)
{
    _id = num;
    _position = position;
    _color = color;
    _moveSpeed = 0.04f;
    _rotationSpeed = 5;
    _direction = QVector2D(0, 1);
    _rotateLeft = false;
    _rotateRight = false;
    _controller = controller;
    _angle = 0;
}

EntityType Player::getEntityType()
{
    return PLAYER;
}

int Player::getId()
{
    return _id;
}

void Player::computeDirection()
{
    if(_rotateLeft)
        rotateLeft();
    if(_rotateRight)
        rotateRight();
}

QVector2D Player::direction() const
{
    return _direction;
}

void Player::setDirection(const QVector2D &direction)
{
    _direction = direction;
}

b2Body *Player::getBody()
{
    return _body;
}

void Player::setBody(b2Body *body)
{
    _body = body;
}

bool Player::getAlive() const
{
    return alive;
}

void Player::setAlive(bool value)
{
    alive = value;
}

void Player::rotate()
{
    Utils::computeDirection(_angle, _direction);
}

void Player::rotateRight()
{
    _angle = -_rotationSpeed;
    rotate();
}

void Player::rotateLeft()
{
    _angle = +_rotationSpeed;
    rotate();
}

QVector3D Player::position() const
{
    return _position;
}

void Player::setPosition(const QVector3D &position)
{
    _position = position;
}

float Player::moveSpeed() const
{
    return _moveSpeed;
}

void Player::setMoveSpeed(float moveSpeed)
{
    _moveSpeed = moveSpeed;
}

QVector4D Player::color() const
{
    return _color;
}

void Player::setColor(const QVector4D &color)
{
    _color = color;
}

Tail* Player::tail()
{
    return &_tail;
}

void Player::setTail(const Tail &tail)
{
    _tail = tail;
}

void Player::updateTail()
{
    _tail.add(_position);
}

void Player::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == _controller[0])
    {
        _rotateLeft = true;
    }
    else if(event->key() == _controller[1])
    {
        _rotateRight = true;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == _controller[0])
    {
        _rotateLeft = false;
    }
    else if(event->key() == _controller[1])
    {
        _rotateRight = false;
    }
}

