#ifndef PARTICLES_H
#define PARTICLES_H

#include <QObject>
#include <QDebug>
#include <QVector4D>
#include <QVector>
#include "particle.h"

const int NB_PARTICLES = 10;

class Particles : public QObject
{
    Q_OBJECT
private:
    QVector<Particle *> _particles;
    QVector4D _color;

    float _timer;
    bool _animationDone;

public:
    Particles();
    Particles(QVector4D color);
    void initParticles(QVector3D pos);

    QVector<QVector3D> getParticlesPosition();

    QVector4D getColor() const;

    bool animationDone();

public slots:
    void update();
};

#endif // PARTICLES_H
