#version 410

in vec3 fE;
in vec3 fL;
in vec2 ftexCoord;

uniform vec4 ambientProduct;
uniform vec4 diffuseProduct;
uniform vec4 specularProduct;
uniform float shininess;

uniform sampler2D colorTexture;
uniform sampler2D colorTextureLayer;

out vec4 fragColor;

vec4 Phong(vec3 N)
{
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    float NdotL = dot(N, L);

    vec3 R = normalize(2.0 * NdotL * N - L);
    float Kd = max(NdotL, 0.0);
    float Ks = (NdotL < 0.0) ? 0.0 : pow(max(dot(R, E), 0.0), shininess);
    vec4 diffuse = Kd * diffuseProduct * texture(colorTexture, ftexCoord);
    vec4 specular = Ks * specularProduct;
    vec4 ambient = ambientProduct;
    return ambient + diffuse + specular;
}

void main()
{
    vec3 N = normalize(texture(colorTextureLayer, ftexCoord).xyz * 2.0 - 1.0);

    if (gl_FrontFacing)
    {
        fragColor = vec4(Phong(N).rgb, 1);
    }
    else
    {
        fragColor = mix(vec4(Phong(-N).rgb, 1), vec4( 0.0, 1.0, 0.0, 1.0), 0.7);
    }
}
