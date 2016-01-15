#ifndef LIGHT_STRUCTURE_H
#define LIGHT_STRUCTURE_H

#include <GL/gl.h>

struct LightInfo {
  GLfloat position[4];
  GLfloat ambiant[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
};

struct SpotLightInfo : virtual LightInfo {
  GLfloat cutOff = 180;
  GLfloat spotDirection[3];
  GLfloat spotExponent = 0.0;
};

struct CompleteLightInfo : virtual LightInfo {
  GLfloat constantAttenuation = 1.0;
  GLfloat linearAttenuation = 0.0;
  GLfloat quadraticAttenuation = 0.0;
};

struct CompleteSpotLightInfo : virtual SpotLightInfo, virtual CompleteLightInfo {

};

#endif
