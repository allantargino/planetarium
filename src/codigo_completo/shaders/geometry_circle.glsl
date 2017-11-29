#version 400

layout (points) in;
layout (line_strip, max_vertices = 101) out;

in vec3 vs_color[];
in float vs_radius[];

out vec3 fs_color;

const float pi = 3.1415926f;
const float divisions = 100.0f;

void main()
{
    for(int i = 0; i < gl_in.length(); i++)
    {
        fs_color = vs_color[i];
        gl_PointSize = 3.0f;

        //float radius = 0.3f;
        float radius = vs_radius[i];

        float pi_2 = pi * 2;
        float step = pi_2 / divisions;
        float complete_circle = pi_2+step;

        for(float j = 0; j <= complete_circle; j+=step)
        {
            gl_Position = gl_in[i].gl_Position + vec4(gl_in[i].gl_Position.x+ radius*cos(j), gl_in[i].gl_Position.y + radius*sin(j), gl_in[i].gl_Position.z, 1.0);
            EmitVertex();
        }

        EndPrimitive();
    }
}
