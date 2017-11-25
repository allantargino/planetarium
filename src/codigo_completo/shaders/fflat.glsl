#version 410

flat in vec4 inoutColor;

out vec4 myfragcolor;

void main()
{
    myfragcolor = inoutColor;
}
