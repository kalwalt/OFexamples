#version 110

uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform sampler2D glossMap;

varying vec3 lightDir;
varying vec3 halfVector;
varying vec3 n;

void main()
{
    float intensity;
    vec3 c = vec3(0.0,0.0,0.0);
    vec3 n = normalize(texture2D(normalMap, gl_TexCoord[0].st).xyz * 2.0 - 1.0);
    vec3 l = normalize(lightDir);
    vec3 h = normalize(halfVector);
    intensity = max(dot(lightDir,normalize(n)),0.0);

    float nDotL = max(0.0, dot(n, l));
    float nDotH = max(0.0, dot(n, h));
    float power = (nDotL == 0.0) ? 0.0 : pow(nDotH, gl_FrontMaterial.shininess);

    vec4 ambient = gl_FrontLightProduct[0].ambient;
    vec4 diffuse = gl_FrontLightProduct[0].diffuse * nDotL;
    vec4 specular = gl_FrontLightProduct[0].specular * power;
    vec4 specularMap = texture2D(glossMap, gl_TexCoord[0].st);
    vec4 color = gl_FrontLightModelProduct.sceneColor + ambient + diffuse + specular*specularMap;
    float coef = smoothstep(1.0,0.2,intensity);
    c += coef *  vec3(color * texture2D(colorMap, gl_TexCoord[0].st));

    //gl_FragColor = color * texture2D(colorMap, gl_TexCoord[0].st);
    gl_FragColor = vec4(c,0);
}
