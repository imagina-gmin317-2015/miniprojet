#include "border.h"

Border::Border()
{}



Border::Border(QVector<QVector2D> pos)
{
    _positions = pos;
}

EntityType Border::getEntityType()
{
    return BORDER;
}

QVector<QVector2D> Border::getPositions() const
{
    return _positions;
}

void Border::setPositions(const QVector<QVector2D> &positions)
{
    _positions = positions;
}

