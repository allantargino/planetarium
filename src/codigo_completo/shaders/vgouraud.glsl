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

out vec4 inoutColor;

void main()
{
    vec4 eyePosition = view * model * vPosition;
    vec3 N = normalize(mat3(view) * normalMatrix * vNormal);
    vec3 L = normalize(lightPosition.xyz - eyePosition.xyz);
    vec3 E = normalize(-eyePosition.xyz);

    float NdotL = dot(N, L);

    vec3 R = normalize(2.0 * NdotL * N - L);
    float Kd = max(NdotL, 0.0);
    float Ks = (NdotL < 0.0) ? 0.0 : pow(max(dot(R, E), 0.0), shininess);

    vec4 diffuse  = Kd * diffuseProduct;
    vec4 specular = Ks * specularProduct;
    vec4 ambient  = ambientProduct;

    inoutColor = vec4((ambient + diffuse+specular).rgb, 1.0);
    gl_Position = projection * eyePosition;
}
