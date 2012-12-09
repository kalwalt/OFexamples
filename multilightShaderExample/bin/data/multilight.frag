#version 130
//#extension GL_gpu_shader4 :enable
#define NUM_LIGHTS 3
varying vec3 normal;
varying vec3 lightDir[NUM_LIGHTS];
varying vec3 eyeVec;
uniform int numLights;
void main (void)
{
  vec4 final_color = gl_FrontLightModelProduct.sceneColor;
   //vec4 final_color = vec4(120,120,10,255.0);

  vec3 N = normalize(normal);
  int i;
  for (i=0; i<numLights; ++i)
  {
    vec3 L = normalize(lightDir[i]);
    float lambertTerm = dot(N,L);
    if (lambertTerm > 0.0f)
    {
      final_color += gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
      vec3 E = normalize(eyeVec);
      vec3 R = reflect(-L, N);
      float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );
      final_color += gl_LightSource[i].specular * gl_FrontMaterial.specular * specular;
      // final_color += gl_LightSource[i].specular * gl_FrontMaterial.specular ;
    }
  }
  gl_FragColor = final_color;
}
