#include "utils.h"

void Utils::computeDirection(float degreeAngle, QVector2D &direction)
{
    float angle = (degreeAngle * PI) / 180.0;
    QVector2D dir = direction;
    direction.setX((dir.x() * cos(angle)) - (dir.y() * sin(angle)));
    direction.setY((dir.x() * sin(angle)) + (dir.y() * cos(angle)));
}

