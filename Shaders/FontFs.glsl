#version 330

struct Color
{
    sampler2D font_texture;
    vec3 font_color;
};

in VS_OUT
{
    vec2 texture_coordinates;
} fs_in;

out vec4 frag_colour;

uniform Color color;

void main()
{
    float texel_R = texture(color.font_texture, fs_in.texture_coordinates).r;
    frag_colour = vec4(1.0, 1.0, 1.0, texel_R) * vec4(color.font_color, 1.0);
}