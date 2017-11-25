#version 410

in vec3 reflectDir;

uniform samplerCube cubeMapTex;

out vec4 fragColor;

void main()
{
    vec4 cubeMapColor = texture(cubeMapTex, reflectDir);
    fragColor = cubeMapColor;
}
