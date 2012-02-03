//#extension GL_ARB_texture_rectangle : enable

varying vec3 vertex_light_position;
varying vec3 norm;
// http://www.swiftless.com/tutorials/glsl/7_texturing.html ///
void main(void) {
	 // Set the front color to the color passed through with glColor*f
    gl_FrontColor = gl_Color;

	 // Calculate the light position for this vertex
    vertex_light_position = normalize(gl_LightSource[0].position.xyz);

	norm = -1.0 * gl_NormalMatrix * gl_Normal;

	/* Transform vertices and pass on texture coordinates */
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;

}

