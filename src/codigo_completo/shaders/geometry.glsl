#version 410

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in vec3 fN[];
in vec3 fE[];
in vec3 fL[];
in vec2 ftexCoord[];

out vec3 gN;
out vec3 gE;
out vec3 gL;
out vec2 gtexCoord;

void main(void)
{
    int i;

    for (i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        gN = fN[i];
        gE = fE[i];
        gL = fL[i];
        gtexCoord = ftexCoord[i];
        EmitVertex();
    }
    EndPrimitive();
}
