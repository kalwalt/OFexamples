/*
Copyright (c) 2007 dhpoware. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

-------------------------------------------------------------------------------

Tangent space normal mapping shader for a single directional light source.

The tangent vector is passed to the vertex shader in gl_MultiTexCoord1. The
tangent vector is assumed to be a four component vector. The tangent vector's
w component indicates the handedness of the local tangent space at this vertex.
The handedness is used to calculate the bitangent vector. The reason for the
inclusion of the handedness component is to allow for triangles with mirrored
texture mappings.

The vertex shader transforms the lightDir and halfVector vectors into tangent
space using the rotation matrix:

                  [ tangent.x    tangent.y    tangent.z   ]
    tbnMatrix  =  [ bitangent.x  bitangent.y  bitangent.z ]
                  [ normal.x     normal.y     normal.z    ]

Since GLSL matrices are column-major the tbnMatrix is declared as

    mat3 tbnMatrix = mat3(tangent.x, bitangent.x, normal.x,
                          tangent.y, bitangent.y, normal.y,
                          tangent.z, bitangent.z, normal.z);

and using this matrix we can transform vectors into tangent space as follows:
    
    vec3 lightDir = tbnMatrix * gl_LightSource[0].position.xyz;
    
The fragment shader performs standard Blinn-Phong per fragment lighting. The
only thing different is we read the normal from the normal map texture rather
than using an interpolated vertex normal. Prior to being stored in the normal
map texture the normals are in the range [-1,1]. Once stored in the normal map
texture the normals will be compressed to the range [0,255]. When the normals
are accessed from the fragment shader GLSL will remap then to the range [0,1].
To convert the normals back to the original [-1,1] range we need to perform a
scale and a bias. We do this by:
    
    vec3 n = texture2D(normalMap, gl_TexCoord[0].st).xyz * 2.0 - 1.0;

We don't have to worry about transforming any vectors into tangent space in the
fragment shader because we have already done this in the vertex shader.

-------------------------------------------------------------------------------
adapted and added Specular map and smoothstep function factor by kalwalt 21/02/2012
http://www.kalwaltart.it e-mail: info@kalwaltart.it

original demo: http://www.dhpoware.com/demos/glslNormalMapping.html
*/


#version 110

varying vec3 lightDir;
varying vec3 halfVector;
varying vec3 n;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;

    vec3 n = normalize(gl_NormalMatrix * gl_Normal);
    vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
    vec3 b = cross(n, t) * gl_MultiTexCoord1.w;

    mat3 tbnMatrix = mat3(t.x, b.x, n.x,
                          t.y, b.y, n.y,
                          t.z, b.z, n.z);

    lightDir = gl_LightSource[0].position.xyz;
    lightDir = tbnMatrix * lightDir;

    halfVector = gl_LightSource[0].halfVector.xyz;
    halfVector = tbnMatrix * halfVector;
}
