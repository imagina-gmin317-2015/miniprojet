#ifndef BONUS_H
#define BONUS_H

#include <food.h>

#include <QObject>
#include <QVector>
#include <QVector4D>
#include <ctime>
#include <iostream>

const int TIME_TO_SPAWN_FOOD = 3.0f;
const int FOOD_FILETIME = 10.0f;

class Bonus : public QObject
{
    Q_OBJECT
private:
    QVector<Food *> _bonus;
    float _timerFood;

public:
    Bonus();
    void addBonus(Food *food);

    QVector<QVector3D> getPositions();
    QVector<QVector4D> getColors();

public slots:
    void update();

signals:
    void addBonusSignal(Food *food);
};

#endif // BONUS_H
