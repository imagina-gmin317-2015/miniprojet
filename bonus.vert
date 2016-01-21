attribute vec4 posAttr;
attribute vec4 colAttr;
uniform mat4 matrix;
varying vec4 col;

void main()
{
    gl_PointSize = 15.0;
    gl_Position = matrix * posAttr;
    col = colAttr;
}

