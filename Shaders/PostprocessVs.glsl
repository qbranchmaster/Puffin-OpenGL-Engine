#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 tex_coord;

out VS_OUT
{
    vec2 tex_coord;
} vs_out;

void main()
{
    vs_out.tex_coord = tex_coord;
    gl_Position = vec4(position, 1.0f);
}