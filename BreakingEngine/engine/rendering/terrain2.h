#ifndef TERRAIN2_H
#define TERRAIN2_H

#include "mesh.h"
#include <QImage>
#include "engine/opengl/vertex.h"

namespace BreakingEngine{
    class Terrain2 : public virtual Mesh
    {
        int width;
        int height;
    public:
        Terrain2(QImage heightMap);
        Terrain2(QImage heightMap, QImage colorMap);
        Terrain2(QString filename);

    private:
        virtual void init(int width, int height, QImage heightMap);

        virtual void init(int width, int height, QImage heightMap, QImage colorMap);
    };
}

#endif // TERRAIN_H
