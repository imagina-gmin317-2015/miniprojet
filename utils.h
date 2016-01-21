#ifndef UTILS_H
#define UTILS_H

#include <QVector2D>
#include <cmath>

const float PI = 3.14159265;

class Utils
{
public:
    static void computeDirection(float degreeAngle, QVector2D &direction);
};

#endif // UTILS_H
