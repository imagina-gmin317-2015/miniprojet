#ifndef PHYSIC_H
#define PHYSIC_H

#include <QVector>
#include <QDebug>
#include <QThread>
#include <player.h>
#include <border.h>
#include <Box2D/Box2D.h>
#include "colliderclass.h"
#include "food.h"

const float PLAYER_RADIUS=0.01;

class Physic : public QObject
{
    Q_OBJECT
private :
    b2World _world;
    Border *_border;
    b2Body* _borderBody;
    QVector<Player *> _players;
    QVector<b2Body *> _playersBody;
    QVector<b2Body *> _tailsBody;
    ColliderClass myColliderInstance;
    QVector<b2Body *> _binBodies;
    QVector<b2Body *> _bonusBody;
    bool pause = false;


public:
    Physic();
    Physic(QVector<Player *> listPlayer);

    void init();
    void initBorder();
    void initPlayer();
    void initTail();
    void addFood(Food *food);
    void deleteFood(Food *foot);
    void updateTail();
    void updateDirection();
    void resetWorldContent();



    QVector<Player *> players();
    void setPlayers(const QVector<Player *> &players);
    ColliderClass* getMyColliderInstance();
    b2World* getWorld() ;
    Border *getBorder() const;
    void setBorder(Border *border);

    QVector<b2Body *> getPlayersBody();
    void setPlayersBody(const QVector<b2Body *> &playersBody);

    QVector<b2Body *> getTailsBody();
    void setTailsBody(const QVector<b2Body *> &tailsBody);

public slots:
    void tick();
    void addToBin(b2Body *body);

};


#endif // PHYSIC_H
