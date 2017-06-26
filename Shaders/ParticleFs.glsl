#version 330 core

in vec2 texture_coords;

out vec4 frag_colour;

uniform sampler2D particle_texture;

void main()
{
    vec4 texel = texture(particle_texture, texture_coords);
    frag_colour = texel;
}