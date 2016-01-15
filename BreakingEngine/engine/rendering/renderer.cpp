#include "renderer.h"
#include "engine/core/camera.h"
#include "engine/core/gameobject.h"
#include "engine/core/component.h"
#include "engine/opengl/bspmanager.h"

Renderer::Renderer(GameObject* parent) : Component(parent)
{
    setObjectName("rendering/Renderer");

}

Renderer::~Renderer() {

}

void Renderer::render()
{
    glColor3f(mainColor.x(),mainColor.y(),mainColor.z());
    if(m_breaking_shader_program==nullptr)
        m_breaking_shader_program = BSPManager::Instance()->getDefaultBreakingShaderProgram();


    //Bind Shader
    m_breaking_shader_program->bind();

    //Compute matrix
    QMatrix4x4 projection;
    QMatrix4x4 view;
    view = Camera::main_camera->viewMatrix();
    projection = Camera::main_camera->projectionMatrix();


    //Link matrix to shader (compute mvp)
    m_breaking_shader_program->m_shader->setUniformValue(m_breaking_shader_program->m_shader->uniformLocation("color"), mainColor);
    m_breaking_shader_program->computeWith(projection,view,transform()->modelMatrix());
    //Draw the content in model view
    glPushMatrix();
    renderContent();
    glPopMatrix();
    //Release Shader
    m_breaking_shader_program->release();
}
