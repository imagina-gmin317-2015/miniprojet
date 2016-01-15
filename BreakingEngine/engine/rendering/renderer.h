#ifndef RENDERER_H
#define RENDERER_H

#include "engine/core/gameobject.h"
#include "engine/core/component.h"
#include "engine/core/transform.h"
#include <QMatrix4x4>
#include <QVector4D>
#include "engine/opengl/breakingshaderprogram.h"

class Renderer : public virtual Component
{
protected:
public:
    BreakingShaderProgram* m_breaking_shader_program=nullptr;
    Renderer(GameObject* parent=0);
    QVector4D mainColor = QVector4D(1.0f,1.0f,1.0f,1.0f);

    virtual void init() = 0;
    virtual ~Renderer();
    virtual void renderContent() = 0;
    virtual void render();
};

#endif // RENDERER_H
