#ifndef MATERIAL_STRUCTURE_H
#define MATERIAL_STRUCTURE_H

#include <GL/gl.h>

struct MaterialInfo {
  /// Ambient color of material
  GLfloat ambient[4];
  /// Diffuse color of material
  GLfloat diffuse[4];
  /// Specular color of material
  GLfloat specular[4];
  //Shininess of material
  GLfloat shininess = 0.0;
  //Light emission of material
  GLfloat emission[4];
};


#endif
