attribute vec4 posAttr;
uniform vec4 playerPosition;
uniform vec4 colUni;
uniform mat4 matrix;
varying vec4 col;


void main()
{
    gl_Position = matrix * posAttr;
//    float dist = distance(posAttr , playerPosition);
    col=colUni;
//    if (dist < 1.3)
//        col = colUni;
//    else
//        col = vec4(0.6, 0.6, 0.6, 0.8);
}
