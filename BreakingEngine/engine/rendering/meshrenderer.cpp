#include "meshrenderer.h"

MeshRenderer::MeshRenderer()
{

}

void MeshRenderer::renderContent() {


    if(mesh== nullptr || mesh->arrayBuf.size() <=0){
        return;
    }

    // Tell OpenGL which VBOs to use
    mesh->arrayBuf.bind();
    mesh->indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = m_breaking_shader_program->attributeLocation("position");
    m_breaking_shader_program->shader()->enableAttributeArray(vertexLocation);
    m_breaking_shader_program->shader()->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    if(vertexLocation==-1)
        qDebug() << "subcolor unknown";
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate color data
    int colorLocation = m_breaking_shader_program->attributeLocation("subcolor");
    if(colorLocation==-1)
        qDebug() << "subcolor unknown";
    m_breaking_shader_program->shader()->enableAttributeArray(colorLocation);
    m_breaking_shader_program->shader()->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = m_breaking_shader_program->attributeLocation("texAttr");
    if(texcoordLocation!=-1) {
    m_breaking_shader_program->shader()->enableAttributeArray(texcoordLocation);
    m_breaking_shader_program->shader()->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2,sizeof(VertexData));
    }

    // Offset for texture coordinate
    offset += sizeof(QVector2D);

    // Tell OpenGL programmable pipeline how to locate vertex normals data
    int normalLocation = m_breaking_shader_program->attributeLocation("normal");
    m_breaking_shader_program->shader()->enableAttributeArray(normalLocation);
    m_breaking_shader_program->shader()->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3,sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, mesh->indexBuf.size(), GL_UNSIGNED_SHORT, 0);
}
