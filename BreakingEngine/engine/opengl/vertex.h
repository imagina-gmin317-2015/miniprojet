#ifndef VERTICES_STRUCTURE_H
#define VERTICES_STRUCTURE_H

#include <GL/gl.h>
#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector3D color;
    QVector2D texCoord;
    QVector3D normal;
};

struct OldVertexData {
  GLfloat coordinates[3];
};

struct ColorVertexData : virtual OldVertexData {
  GLfloat color[4];
};

struct TextureVertexData : virtual OldVertexData {
  GLfloat texCoordinates[2];
};

struct NormalVertexData : virtual OldVertexData {
  GLfloat normal[3];
};

struct CompleteColorVertexData : virtual ColorVertexData, virtual NormalVertexData {

};

struct CompleteTextureVertexData : virtual TextureVertexData,
virtual ColorVertexData, virtual NormalVertexData {

};

struct OldCompleteVertexData {
  GLfloat coordinates[3];
  GLfloat normal[3];
  GLfloat color[4];
  GLfloat texCoordinates[2];
};



#endif
