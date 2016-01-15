#version 130
//precision highp float;
attribute vec3 position;
attribute vec3 normal;
attribute vec3 subcolor;

uniform vec4 color;
uniform float time;
uniform mat3 normalMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;

varying vec3 fNormal;
varying vec3 fPosition;
varying vec3 fColor;

void main(void)
{
    fNormal = normal;
    vec3 npos = vec3(position.x,position.y+position.y*(sin(time)/5.0),position.z);
    vec4 pos = mvpMatrix * vec4(npos,1);
    fPosition = pos.xyz;
    fColor = subcolor;
    gl_FrontColor = vec4(color.xyz,0.5);
    gl_Position = pos;
}

