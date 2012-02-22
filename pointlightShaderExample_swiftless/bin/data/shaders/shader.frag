//shader to use with spot light

varying vec3 vertex_light_position;
varying vec3 vertex_light_half_vector;
varying vec3 vertex_normal;

void main() {
	// Calculate the ambient term
	vec4 ambient_color = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;;

	// Calculate the diffuse term
	vec4 diffuse_color = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	// Calculate the specular value
	vec4 specular_color = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(max(dot(vertex_normal, vertex_light_half_vector), 0.0), gl_FrontMaterial.shininess);

	// Set the diffuse value (darkness). This is done with a dot product between the normal and the light
	// and the maths behind it is explained in the maths section of the site.
	float diffuse_value = max(dot(vertex_normal, vertex_light_position), 0.0);

    //float spotcutoff = gl_LightSource[0].spotCutoff;
	// Set the output color of our current pixel
	gl_FragColor = ambient_color + diffuse_color * diffuse_value + specular_color;
}
