#include "breakingshaderprogram.h"
#include "engine/core/time.h"
BreakingShaderProgram::BreakingShaderProgram()
{

}

bool BreakingShaderProgram::prepareShader(QString vertex, QString fragment)
{
   m_vertex = vertex;
   m_fragment = fragment;
}

BreakingShaderProgram::~BreakingShaderProgram() {

    delete m_shader;
}

bool BreakingShaderProgram::build() {
    qDebug() << "BreakingShaderProgram::build():"<<m_vertex<<":"<<m_fragment;
    m_shader = new QOpenGLShaderProgram();
    QOpenGLShader* shade = new QOpenGLShader(QOpenGLShader::Vertex);
    bool result = shade->compileSourceFile(m_vertex);
    QOpenGLShader* fshade = new QOpenGLShader(QOpenGLShader::Fragment);
    result |= fshade->compileSourceFile(m_fragment);
    if(result) {
        addShader(shade);
        addShader(fshade);
        return true;
    }
    return false;
}

void BreakingShaderProgram::start()
{
    if(!m_shader->link())
    {
        qDebug() << "linking error";
        exit(-100);
    }
}


void BreakingShaderProgram::computeWith(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model) {
    QMatrix4x4 viewProjectionMatrix = projection * view;
    QMatrix4x4 modelViewMatrix = view * model;
    QMatrix4x4 mvpMatrix = projection * view * model;

    m_shader->setUniformValue(m_shader->uniformLocation("time"),Time::time());
    m_shader->setUniformValue(m_shader->uniformLocation("modelViewMatrix"), modelViewMatrix);
    m_shader->setUniformValue(m_shader->uniformLocation("projectionMatrix"), projection);
    m_shader->setUniformValue(m_shader->uniformLocation("mvpMatrix"),mvpMatrix);
    m_shader->setUniformValue(m_shader->uniformLocation("viewProjectionMatrix"),viewProjectionMatrix);
    m_shader->setUniformValue(m_shader->uniformLocation("viewMatrix"),view);
    m_shader->setUniformValue(m_shader->uniformLocation("modelMatrix"),model);
}

void BreakingShaderProgram::bind() {
    m_shader->bind();
}

void BreakingShaderProgram::release() {
    m_shader->release();
}

bool BreakingShaderProgram::addShader(QOpenGLShader *shader)
{
    return m_shader->addShader(shader);
}

