#version 130
//#extension GL_gpu_shader4 :enable

#define NUM_LIGHTS 3
varying vec3 normal;
varying vec3 lightDir[NUM_LIGHTS];
varying vec3 eyeVec;
uniform int numLights;
void main (void)
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
 // normal = gl_NormalMatrix * gl_Normal;
  normal =   gl_Normal;
  // N = normalize(gl_NormalMatrix * gl_Normal);
  //vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
   vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

  eyeVec = -vVertex.xyz;
  int i;
  for (i=0; i<numLights; ++i)
    lightDir[i] = vec3(gl_LightSource[i].position.xyz - vVertex.xyz);
    //lightVector[i] = lightPosition[i] - gl_Vertex.xyz;
}
