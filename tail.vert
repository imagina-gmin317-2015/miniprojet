attribute vec4 posAttr;
uniform vec4 colAttr;
uniform mat4 matrix;
varying vec4 col;

void main()
{
    gl_PointSize = 4.0;
    gl_Position = matrix * posAttr;
    col = colAttr;
}
