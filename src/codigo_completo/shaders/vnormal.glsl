#version 410

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec4 ambientProduct;
uniform vec4 diffuseProduct;
uniform vec4 specularProduct;
uniform float shininess;
uniform vec4 lightPosition;
out vec4 v2fcolor;

void main()
{
    gl_Position = projection * view * model * vPosition;
    v2fcolor = vec4(abs(vNormal), 1);
}
