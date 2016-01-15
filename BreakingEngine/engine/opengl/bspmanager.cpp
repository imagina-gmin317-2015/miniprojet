#include "bspmanager.h"
#include <QOpenGLShader>
QString BSPManager::defaultShaderName = "BreakingShader";


BSPManager::BSPManager()
{
    BreakingShaderProgram* bsp = new BreakingShaderProgram();
    m_shaders[BSPManager::defaultShaderName] = bsp;

    bsp->prepareShader(":/resources/shaders/breaking.vs",":/resources/shaders/breaking.fs");
}

BSPManager::~BSPManager()
{
    foreach(QString name, m_shaders.keys()) {
        BreakingShaderProgram* bsp = m_shaders[name];
        m_shaders.remove(name);
        delete bsp;
    }
}

