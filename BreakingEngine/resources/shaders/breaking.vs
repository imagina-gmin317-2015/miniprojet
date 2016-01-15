#version 130
//precision highp float;
attribute vec3 position;
attribute vec3 normal;

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

void main()
{
    fNormal = normalize(normalMatrix * normal);
    vec4 pos = mvpMatrix * gl_Vertex;
    fPosition = pos.xyz;
    gl_FrontColor = gl_Color;
    gl_Position = pos;
}
