#include "colliderclass.h"

ColliderClass::ColliderClass()
{

}

void ColliderClass::BeginContact(b2Contact *contact)
{
    Entity* bodyUserAData = static_cast<Entity*> (contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* bodyUserBData = static_cast<Entity*> (contact->GetFixtureB()->GetBody()->GetUserData());

    CheckPlayerCollision(bodyUserAData, bodyUserBData);
    CheckPlayerCollision(bodyUserBData, bodyUserAData);
}

void ColliderClass::CheckPlayerCollision(Entity* bodyUserAData, Entity* bodyUserBData)
{
    if((bodyUserAData->getEntityType() == PLAYER && bodyUserBData->getEntityType() == BORDER) ||
            (bodyUserAData->getEntityType() == PLAYER && bodyUserBData->getEntityType() == PLAYER) ||
            (bodyUserAData->getEntityType() == PLAYER && bodyUserBData->getEntityType() == TAIL))
    {
        qDebug()<<"in collider"<<(int) bodyUserBData->getEntityType();
        Player *player = static_cast<Player*> (bodyUserAData);
        emit playerExplodes(player);
    }

    if((bodyUserAData->getEntityType() == PLAYER && bodyUserBData->getEntityType() == FOOD))
    {
        qDebug()<<"in collider"<<(int) bodyUserBData->getEntityType();
        Player *player = static_cast<Player*> (bodyUserAData);
        Food *food = static_cast<Food *> (bodyUserBData);
        emit destroyBody(food->getBody());
        qDebug()<<"here B";
    }
}

