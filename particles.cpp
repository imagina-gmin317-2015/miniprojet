#include "particles.h"

QVector4D Particles::getColor() const
{
    return _color;
}

bool Particles::animationDone()
{
    return _animationDone;
}

Particles::Particles()
{
    _timer = 0.0f;
    _color = QVector4D(1.0f, 0.0f, 0.0f, 1.0f);
}

Particles::Particles(QVector4D color)
{
    _timer = 0.0f;
    _color = color;
}

void Particles::initParticles(QVector3D pos)
{
    _timer = 0.0f;
    _particles.clear();
    _animationDone = false;
    for(int i = 0; i < NB_PARTICLES; i++)
    {
        _particles.push_back(new Particle(pos));
    }
}

QVector<QVector3D> Particles::getParticlesPosition()
{
    QVector<QVector3D> particlesPosition = QVector<QVector3D>();
    for(Particle *p : _particles)
    {
        particlesPosition.push_back(p->getPosition());
    }
    return particlesPosition;
}

void Particles::update()
{
    _timer += 0.4f;
    for(int i = 0; i < _particles.size(); i++)
    {
        if(_timer > _particles[i]->getlifetime())
        {
            _particles.remove(i);
        }
        else
        {
            _particles[i]->update();
        }
    }
    if(_particles.size() < 1)
        _animationDone = true;
}
