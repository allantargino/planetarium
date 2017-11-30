# version 400
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
layout (location = 2) in float in_Radius;

out vec3 vs_color;
out float vs_radius;

void main(void)
{
    //gl_PointSize = 20.0;
    gl_Position = vec4(in_Position.xyz, 1.0);
    vs_color = in_Color;
    vs_radius = in_Radius;
}
