/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */
#ifndef COMMONSTRUCT
#define COMMONSTRUCT

#include <QVector3D>
#include <QVector2D>

///Structure d'un vertice
struct VertexData
{
    QVector3D position;
    QVector3D color;
    QVector2D texCoord;
    QVector3D normal;
};
/// Coordonnées pour QuadTree
enum Cardinal { NONE = -1, NORTH_WEST = 0, NORTH_EAST = 1, SOUTH_EAST = 2, SOUTH_WEST = 3 };

#endif // COMMONSTRUCT

