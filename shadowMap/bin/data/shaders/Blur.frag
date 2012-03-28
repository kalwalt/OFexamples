/////////////////////////////////////////////////
// 7x1 gaussian blur fragment shader
/////////////////////////////////////////////////

//varying vec2 v_Coordinates;

uniform vec2 ScaleU;
uniform sampler2D textureSource;

// Portability prevented us from using a const array of vec2
// Mac shader compiler don't support it.
/*
const vec2 gaussFilter[7] = 
{ 
	-3.0,	0.015625,
	-2.0,	0.09375,
	-1.0,	0.234375,
	0.0,	0.3125,
	1.0,	0.234375,
	2.0,	0.09375,
	3.0,	0.015625
};
*/

void main()
{
	vec4 color = vec4(0.0);
	//for( int i = 0; i < 9; i++ )
	//{
	//	color += texture2D( textureSource, gl_TexCoord[0].st + vec2( gaussFilter[i].x*ScaleU.x, gaussFilter[i].x*ScaleU.y ) )*gaussFilter[i].y;
	//}
	color += texture2D( textureSource, gl_TexCoord[0].st + vec2( -3.0*ScaleU.x, -3.0*ScaleU.y ) ) * 0.015625;
	color += texture2D( textureSource, gl_TexCoord[0].st + vec2( -2.0*ScaleU.x, -2.0*ScaleU.y ) )*0.09375;
	color += texture2D( textureSource, gl_TexCoord[0].st + vec2( -1.0*ScaleU.x, -1.0*ScaleU.y ) )*0.234375;
	color += texture2D( textureSource, gl_TexCoord[0].st + vec2( 0.0 , 0.0) )*0.3125;
	color += texture2D( textureSource, gl_TexCoord[0].st + vec2( 1.0*ScaleU.x,  1.0*ScaleU.y ) )*0.234375;
	color += texture2D( textureSource, gl_TexCoord[0].st + vec2( 2.0*ScaleU.x,  2.0*ScaleU.y ) )*0.09375;
	color += texture2D( textureSource, gl_TexCoord[0].st + vec2( 3.0*ScaleU.x, -3.0*ScaleU.y ) ) * 0.015625;

	gl_FragColor = color;
}