attribute vec4 posAttr;
uniform mat4 matrix;
uniform vec4 colUni;
varying vec4 col;

void main(void)
{
    gl_Position = matrix * posAttr;
    col = colUni;
}

