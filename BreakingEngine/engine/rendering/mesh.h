#ifndef MESH_H
#define MESH_H

#include "engine/core/common.h"
#include "engine/opengl/vertex.h"

#include <QOpenGLBuffer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QWidget>
#include <QOpenGLContext>
#include <QObject>

class Mesh : public QWidget
{
protected:
    int size=-1;
    int indexSize =-1;
public:


    VertexData* vertices = nullptr;
    GLushort* indices = nullptr;
    QOpenGLBuffer arrayBuf = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);                     //Buffer du tableau de vertices
    QOpenGLBuffer indexBuf = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);                     //Buffer du tableau d'indices

    Mesh(QWidget* parent=0) : QWidget(parent) {

    }

    virtual ~Mesh() {
        clean();
        arrayBuf.destroy();
        indexBuf.destroy();
    }

    virtual void build() {
        arrayBuf.create();
        indexBuf.create();
        arrayBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        indexBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
        bind();
    }

    void clean() {
        size = -1;

        if(nullptr != vertices)
            delete vertices;
        if(nullptr != indices)
            delete indices;
    }

    void init(int verticesSize) {
        clean();
        size = verticesSize;
        vertices = new VertexData[size];

    }

    void init(int verticesSize, VertexData* vertices2) {
        clean();
        size = verticesSize;
        vertices = vertices2;
    }

    void init(int verticesSize, VertexData* vertices2,int indexesSize, GLushort* indexes) {
        clean();
        size = verticesSize;
        indexSize = indexesSize;
        vertices = vertices2;
        indices = indexes;
    }

    void bind() {
        arrayBuf.bind();
        qDebug() << "Mesh:bind():vertexdata:"<<size <<"(total ="<<size * sizeof(VertexData) <<" ; VertexData:"<<sizeof(VertexData)<<")";
        arrayBuf.allocate(vertices, size * sizeof(VertexData));
        indexBuf.bind();
        qDebug() << "Mesh:bind():indexes:"<<indexSize <<"(total ="<<indexSize * sizeof(GLushort) <<" ; VertexData:"<<sizeof(GLushort)<<")";
        indexBuf.allocate(indices, indexSize * sizeof(GLushort));
    }
public slots:
signals:
};

#endif // MESH_H
