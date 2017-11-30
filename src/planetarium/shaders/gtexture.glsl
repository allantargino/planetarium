#version 400

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

    vec4 pos = vec4(0.5,0.5,0,1);  //introduce a single vertex at the origin
    for(float i = 0; i < 6.38 ; i+=0.1)  //generate vertices at positions on the circumference from 0 to 2*pi
    {
        gl_Position = vec4(pos.x+0.5*cos(i),pos.y+0.5*sin(i),pos.z,1.0);   //circle parametric equation
        EmitVertex();
    }

    EndPrimitive();
}
