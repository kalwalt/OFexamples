//#extension GL_ARB_texture_rectangle : enable

//uniform sampler2DRect baseMap;
uniform sampler2D baseMap;
varying vec3 vertex_light_position;
varying vec3 norm;

void main(void) {

	/* Extract colors from baseMap */
	//vec4 baseColor      = texture2DRect( baseMap, gl_TexCoord[0].st );
	vec4 baseColor      = texture2D( baseMap, gl_TexCoord[0].st );

	vec3 normal			= normalize( norm );
	float diffuse_value = max(dot(normal, vertex_light_position), 0.0);

	gl_FragColor		= diffuse_value * baseColor;

}
