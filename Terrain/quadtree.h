/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */

#ifndef QUADTREE_H
#define QUADTREE_H

#include <QVector3D>
#include <QtGui/QOpenGLFunctions>
#include <vector>

#include "voxel.h"
#include "commonstruct.h"

/**
 * @brief The Quadtree class est la structure d'accélération permettant de décimer notre terrain.
 *
 */
class Quadtree
{
public:
    Quadtree(VertexData* vert, float delt, int resolution, int terrain_width);

    void QuadtreeStatic(Voxel* v, int offsetX, int offsetY);
    void Reset(VertexData* vert);
    void DisplayQuadtree();

    std::vector<Voxel*> getVoxels() const { return voxels; };
    int getNbVoxels() const { return voxels.size(); };

    float getDelta() const { return delta; };

    void setDelta(float d) { delta = d; };
    void setResolution(int d) { resolution = d; };

    void ShowVoxelStat();

private:
    Voxel* Subdivide(Voxel* v);
    float ComputeHeightAverage(int offsetX, int offsetY, int length);

private:
    float delta;
    int resolution;
    int terrain_width;

    VertexData* vertices;

    std::vector<Voxel*> voxels;

signals:

public slots:
};

#endif // QUADTREE_H
