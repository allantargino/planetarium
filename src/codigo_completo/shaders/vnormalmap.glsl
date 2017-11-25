#version 410

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec4 vTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;
uniform vec4 lightPosition;

out vec3 fE;
out vec3 fL;
out vec2 ftexCoord;

void main()
{
    vec3 bitangent = vTangent.w * cross(vNormal, vTangent.xyz);
    vec3 T = normalMatrix * vTangent.xyz;
    vec3 B = normalMatrix * bitangent;
    vec3 N = normalMatrix * vNormal;

    mat3 TBN = mat3(T.x, B.x, N.x, T.y, B.y, N.y, T.z, B.z, N.z);
    vec4 eyePosition = view * model * vPosition;
    fL = TBN * (lightPosition.xyz - eyePosition.xyz);
    fE = TBN * (-eyePosition.xyz);
    ftexCoord = vTexCoords;
    gl_Position = projection * eyePosition;
}
