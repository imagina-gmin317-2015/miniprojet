#ifndef MESHRENDERER_H
#define MESHRENDERER_H


#include "renderer.h"
#include "mesh.h"

class MeshRenderer : public virtual Renderer
{
public:
    Mesh* mesh = nullptr;
    MeshRenderer();
    virtual void init() {
        if(mesh!=nullptr)
            mesh->build();
    }

    virtual void renderContent();

};

#endif // MESHRENDERER_H
