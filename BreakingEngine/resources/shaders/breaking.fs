#version 130
//precision highp float;
uniform float time;
uniform vec2 resolution;

varying vec3 fPosition;
varying vec3 fNormal;

void main()
{

  gl_FragColor = gl_Color;
///  float intensity;
///  vec3 lightDir = normalize(gl_LightSource[0].position.xyz - fPosition);
///  intensity = dot(normalize(lightDir),normalize(fNormal));
///  float factor = 1.0;

///  vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(fNormal,lightDir), 0.0);
///  Idiff = clamp(Idiff, 0.0, 1.0);
///  gl_FragColor = Idiff;
///  if (intensity < 0.5)
///      factor = 0.5;


///  vec4 col2 = vec4(gl_Color.xyz,1);
///  col2 *= vec4(factor, factor, factor, 1);
///  //float s = sin(time);
///  //col2 = vec4(s,0,0,1);
///  //gl_FragColor = vec4(fNormal,1);
///  gl_FragColor = col2;
}
