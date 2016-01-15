/**
 *  @Author David Lonni
 *  @Author Sébastien Beugnon
 *  @Author Maxime Demaille
 */
#include <QDebug>
#include <qmath.h>

#include "quadtree.h"

Quadtree::Quadtree(VertexData* vert, float delt, int res, int terrain_w){
    delta = delt;
    resolution = res;
    terrain_width = terrain_w;

    Reset(vert);
}

Voxel* Quadtree::Subdivide(Voxel* v){
    Voxel* subDivideVoxel = new Voxel[4];

    float l = v->getLength() / 2.f;
    QVector3D pos = v->getPosition();

    subDivideVoxel[0] = Voxel(pos + QVector3D(-l/2.0, 0, -l/2.0), l, Cardinal::NORTH_WEST);
    subDivideVoxel[1] = Voxel(pos + QVector3D(l/2.0, 0, -l/2.0), l, Cardinal::NORTH_EAST);
    subDivideVoxel[2] = Voxel(pos + QVector3D(l/2.0, 0, l/2.0), l, Cardinal::SOUTH_EAST);
    subDivideVoxel[3] = Voxel(pos + QVector3D(-l/2.0, 0, l/2.0), l, Cardinal::SOUTH_WEST);

    return subDivideVoxel;
}

float Quadtree::ComputeHeightAverage(int offsetX, int offsetY, int length)
{
    float sum = 0.f;
    int cpt = 0;

    for(int i = offsetY ; i < length + offsetY ; i++){
        for(int j = offsetX ; j < length + offsetX ; j++){
            sum += vertices[i * terrain_width + j].position.y();
            cpt++;
        }
    }
    float moy = sum / cpt;

    float ecartType = 0.f;

    for(int i = offsetY ; i < length + offsetY ; i++){
        for(int j = offsetX ; j < length + offsetX ; j++){
            ecartType += pow(vertices[i * terrain_width + j].position.y() - moy, 2.f);
        }
    }

    ecartType = sqrt(ecartType / cpt);

    return ecartType;
}

void Quadtree::QuadtreeStatic(Voxel* v, int offsetX, int offsetY){
    v->setOffset(offsetX, offsetY);

    if(ComputeHeightAverage(v->getOffsetX(), v->getOffsetY(), v->getLength() * 2) > delta){

        if(v->getLength() > resolution){
            Voxel* subdivide = Subdivide(v);

            for(int i = 0 ; i < 4 ; i++){
                subdivide[i].AddParent(v);
                v->AddChild(&subdivide[i]);
            }

            QuadtreeStatic(&subdivide[0], offsetX, offsetY);
            QuadtreeStatic(&subdivide[1], offsetX + v->getLength(), offsetY);
            QuadtreeStatic(&subdivide[2], offsetX + v->getLength(), offsetY + v->getLength());
            QuadtreeStatic(&subdivide[3], offsetX, offsetY + v->getLength());
        }else{
            voxels.push_back(v);
        }
    }else{
        voxels.push_back(v);
    }

}

void Quadtree::Reset(VertexData* vert)
{
    vertices = vert;
    voxels.clear();
}

void Quadtree::DisplayQuadtree()
{
    for(unsigned int i = 0 ; i < voxels.size() ; i++){
        voxels[i]->DisplayVoxel();
    }
}

void Quadtree::ShowVoxelStat()
{
    for(unsigned int i = 0 ; i < voxels.size() ; i++){
        voxels[i]->DisplayStat();
    }
}
