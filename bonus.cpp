#include "bonus.h"

Bonus::Bonus()
{
    _bonus = QVector<Food *>();
    _timerFood = 0.0f;
}

void Bonus::update()
{
    _timerFood += 0.1f;
    if(_timerFood >= TIME_TO_SPAWN_FOOD)
    {
        _timerFood = 0.0f;
        QVector3D pos;
        pos.setX(-1.0 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX/(1.0 - (-1.0)))));
        pos.setY(-1.0 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX/(1.0 - (-1.0)))));
        Food *food = new Food(pos, FOOD_FILETIME, QVector4D(0.78, 0.23, 0.7, 1.0));
        addBonus(food);
        emit addBonusSignal(food);
    }
    for(int i = 0; i < _bonus.size(); i++)
    {
        if(_bonus[i]->timer() >= _bonus[i]->lifetime())
        {
            _bonus.remove(i);
            continue;
        }
        _bonus[i]->updateTimer();
    }

}

void Bonus::addBonus(Food *food)
{
    _bonus.push_back(food);
}

QVector<QVector3D> Bonus::getPositions()
{
    QVector<QVector3D> positions = QVector<QVector3D>();
    for(int i = 0; i < _bonus.size(); i++)
    {
        positions.push_back(_bonus[i]->position());
    }
    return positions;
}

QVector<QVector4D> Bonus::getColors()
{
    QVector<QVector4D> colors = QVector<QVector4D>();
    for(int i = 0; i < _bonus.size(); i++)
    {
        colors.push_back(_bonus[i]->color());
    }
    return colors;
}
