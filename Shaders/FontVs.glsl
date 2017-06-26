#version 330

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texture_coordinates;

out VS_OUT
{
    vec2 texture_coordinates;
} vs_out;
 
void main() 
{
    vs_out.texture_coordinates = texture_coordinates;
    gl_Position = vec4(position, 1.0);
}