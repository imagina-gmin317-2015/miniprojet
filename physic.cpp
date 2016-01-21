#include "physic.h"

QVector<Player *> Physic::players()
{
    return _players;
}

void Physic::setPlayers(const QVector<Player *> &players)
{
    this->_players = players;
}

ColliderClass* Physic::getMyColliderInstance()
{
    return &myColliderInstance;
}

b2World *Physic::getWorld()
{
    return &_world;
}

Border *Physic::getBorder() const
{
    return _border;
}

void Physic::setBorder(Border *border)
{
    _border = border;
}

QVector<b2Body *> Physic::getPlayersBody()
{
    return _playersBody;
}

void Physic::setPlayersBody(const QVector<b2Body *> &playersBody)
{
    _playersBody = playersBody;
}

QVector<b2Body *> Physic::getTailsBody()
{
    return _tailsBody;
}

void Physic::setTailsBody(const QVector<b2Body *> &tailsBody)
{
    _tailsBody = tailsBody;
}

Physic::Physic():_world(b2Vec2(0.0f, 0.0f))
{

}

Physic::Physic(QVector<Player *> listPlayer):_world(b2Vec2(0.0f, 0.0f))
{
    _players=listPlayer;
}

void Physic::init(){
    _world.SetContactListener(&myColliderInstance);
    initBorder();
    initPlayer();
    initTail();
}

void Physic::initBorder()
{
    b2BodyDef borderBodyDef;
    borderBodyDef.type = b2_staticBody;
    b2ChainShape chain;
    std::vector<b2Vec2> vec;
    for (QVector2D v : _border->getPositions())
        vec.push_back(b2Vec2(v.x(),v.y()));
    chain.CreateLoop(&vec[0], vec.size());
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &chain;
    _borderBody = _world.CreateBody(&borderBodyDef);
    _borderBody->CreateFixture(&fixtureDef);
    _borderBody->SetUserData(_border);
}

void Physic::initPlayer()
{
    b2CircleShape playerShape;
    playerShape.m_p.Set(0, 0); //position, relative to body position
    playerShape.m_radius = PLAYER_RADIUS; // radius

    b2FixtureDef playerFixtureDef;
    playerFixtureDef.shape=&playerShape;
    playerFixtureDef.density=0;

    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;

    _playersBody.clear();
    for (int i=0 ;i <_players.size();i++)
    {
        playerBodyDef.position.Set( _players[i]->position().x()  ,_players[i]->position().y());
        _playersBody<<_world.CreateBody(&playerBodyDef);
        _playersBody[i]->CreateFixture(&playerFixtureDef);
        _playersBody[i]->SetUserData( _players[i]); // Bind du body avec l'objet player
        _players[i]->setBody(_playersBody[i]); // Bind du player avec le body ..
    }
}


void Physic::initTail(){
    b2BodyDef tailBodyDef;
    tailBodyDef.type = b2_staticBody;
    for(Player* player : _players)
    {
        _tailsBody<<_world.CreateBody(&tailBodyDef);
        _tailsBody.last()->SetUserData(player->tail());
        player->tail()->setBody(_tailsBody.last());
    }
}

void Physic::addFood(Food *food)
{
    b2CircleShape bonusShape;
     bonusShape.m_p.Set(0, 0); //position, relative to body position
     bonusShape.m_radius = PLAYER_RADIUS; // radius

     b2FixtureDef bonusFixtureDef;
     bonusFixtureDef.shape=&bonusShape;
     bonusFixtureDef.density=0;

     b2BodyDef bonusBodyDef;
     bonusBodyDef.type = b2_dynamicBody;

     bonusBodyDef.position.Set( food->position().x()  ,food->position().y());
     _bonusBody<<_world.CreateBody(&bonusBodyDef);
     _bonusBody.last()->CreateFixture(&bonusFixtureDef);
     _bonusBody.last()->SetUserData( food); // Bind du body avec l'objet bonus
     food->setBody(_bonusBody.last()); // Bind du bonus avec le body ..

}

void Physic::deleteFood(Food *foot)
{

}

void Physic::updateTail()
{
    static const float minDistance=0.006;
    QVector3D memory;
    std::vector< b2Vec2> vec;
    QVector<QVector3D> tmp, tmp2;
    Tail* tail;

    for (Player *player : _players)
    {
        b2Body * body =player->tail()->getBody();
        if (!player->getAlive()) continue;
        b2Fixture *fixtureA = body->GetFixtureList();
        if (fixtureA!=NULL) body->DestroyFixture(fixtureA);
        tmp.clear();
        tmp2.clear();
        tail=static_cast<Tail*> (body->GetUserData());
        tmp = tail->getChain();
        vec.clear();
        for (QVector3D v : tmp)
            if (v.distanceToPoint(tmp.last())>PLAYER_RADIUS) tmp2.push_back(v);
        if (tmp2.size()<=2) break;
        vec.push_back(b2Vec2(tmp2[0].x(),tmp2[0].y()));
        memory=tmp2[0];

        for (int i = 1;i<tmp2.size();i++){
            if (tmp2[i].distanceToPoint(memory)<minDistance) continue;
            memory=tmp2[i];
            vec.push_back(b2Vec2(tmp2[i].x(),tmp2[i].y()));
        }
        if (vec.size()<2) continue; // Il faut au moins 2 points pour créer une chaine
        b2ChainShape chain;
        chain.CreateChain(&vec[0], vec.size());
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &chain;
        body->CreateFixture(&fixtureDef);
    }
}

void Physic::updateDirection()
{
    for(Player* player : _players)
    {
        player->computeDirection();
    }
}

void Physic::tick()
{
    if (_binBodies.size()!=0){
       // qDebug()<<"its apo of size : "<<_binBodies.size();
        for (b2Body* body : _binBodies) _world.DestroyBody(body);
        _binBodies.clear();
    }

    if (pause) return; // we don't go further if the physics is stopped
    if (_world.GetBodyCount()==0) {
        return;
    }

    float32 timeStep = 30.0f/1000.0f;

    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    updateDirection();
    for (Player* player : _players){
        player->getBody()->SetLinearVelocity(10*b2Vec2(player->direction().x()*player->moveSpeed(),player->direction().y()*player->moveSpeed()));
    }

    _world.Step(timeStep, velocityIterations, positionIterations);
    for (Player* player : _players){
        b2Vec2 position = player->getBody()->GetPosition();
        player->setPosition(QVector3D(position.x, position.y, 0));
    }
    updateTail();



}

void Physic::addToBin(b2Body *body)
{
    _binBodies.push_back(body);
}

void Physic::resetWorldContent()
{
    _binBodies.clear();
    b2Body* body=_world.GetBodyList();
    while (body != NULL){
        _binBodies.push_back(body);
        body = body->GetNext();
    }
}
