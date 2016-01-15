#ifndef FOGINFO_H
#define FOGINFO_H

#include "GL/gl.h"

struct FogInfo {
    GLint mode = GL_EXP;
    GLfloat color[4];
    GLfloat density;
    GLint hint = GL_DONT_CARE;
    GLfloat start;
    GLfloat end;

};

#endif // FOGINFO

