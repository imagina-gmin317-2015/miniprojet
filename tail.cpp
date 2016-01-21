#include "tail.h"

QVector<QVector3D> Tail::getChain() const
{
    return _chain;
}

void Tail::setChain(const QVector<QVector3D> &chain)
{
    _chain = chain;
}


b2Body *Tail::getBody()
{
    return body;
}

void Tail::setBody(b2Body *value)
{
    body = value;
}

Tail::Tail()
{
    
}



EntityType Tail::getEntityType()
{
    return TAIL;
}

int Tail::size()
{
    return _chain.size();
}

void Tail::add(QVector3D chain)
{
    // On ajoute un point à la chaine uniquement si il est "significativement distant" du précédent
    // Sinon, la physique crash : on ne peut pas avoir deux points plus proche que 0.005*0.005
    if (_chain.size()==0) _chain.push_back(chain);
    else if (chain.distanceToPoint(_chain.last())>0.006*0.006) _chain.push_back(chain);

    if (_chain.size()>_nbChain) _chain.removeFirst();
}

