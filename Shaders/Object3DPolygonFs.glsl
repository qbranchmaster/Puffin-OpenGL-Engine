#version 330 core

out vec4 frag_color;

uniform vec3 lines_color;

void main()
{
    frag_color = vec4(lines_color, 1.0f);
}