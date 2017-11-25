#version 410

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 reflectDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

void main()
{
    // https://learnopengl.com/#!Advanced-OpenGL/Cubemaps
    vec4 eyePosition = view * model * vPosition;
    //reflectDir = vec3(model * vPosition);
    vec3 normal = mat3(transpose(inverse(model))) * vNormal;
    reflectDir = reflect(normalize((model * vPosition) - vec4(0,0,1,0)).xyz, normalize(normal).xyz);
    gl_Position = projection * eyePosition;
}
