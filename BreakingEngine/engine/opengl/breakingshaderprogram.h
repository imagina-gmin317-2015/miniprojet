#ifndef BREAKINGSHADERPROGRAM_H
#define BREAKINGSHADERPROGRAM_H

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>

class BreakingShaderProgram
{
    friend class Renderer;
    QOpenGLShaderProgram* m_shader;

    QString m_vertex;
    QString m_fragment;

    int m_uniform_modelview;
    int m_uniform_projection;
    int m_uniform_color;

public:
    BreakingShaderProgram();

    virtual bool prepareShader(QString vertex, QString fragment);

    virtual bool build();
    virtual void start();
    virtual ~BreakingShaderProgram();
    virtual void computeWith(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);
    virtual void bind();
    virtual void release();
    virtual bool addShader(QOpenGLShader *shader);

    QOpenGLShaderProgram* shader() {
        return m_shader;
    }

    virtual int attributeLocation(QString name) {
        return m_shader->attributeLocation(name);
    }
    virtual int uniformLocation(QString name) {
        return m_shader->uniformLocation(name);
    }

};

#endif // BREAKINGSHADERPROGRAM_H
