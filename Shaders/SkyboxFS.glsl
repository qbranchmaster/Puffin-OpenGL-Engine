#version 330 core

struct Fog
{
    bool enabled;
    vec3 color;
    float density;
    float transition_power;
    float height;
};

struct Color
{
    samplerCube cube_texture;
    vec3 filter_color;
    vec3 light_color;
};

in VS_OUT
{
    vec3 texture_coordinates;
} fs_in;

out vec4 frag_color;

uniform Color color;
uniform Fog fog;

vec3 calcFog(vec3 input_color)
{
    vec3 result = mix(input_color, fog.color, fog.density);

    if (fs_in.texture_coordinates.y <= fog.height &&
        fs_in.texture_coordinates.y >= -0.2f)
    {
        float distance = 0.0f;
        if (fs_in.texture_coordinates.y < 0.0f)
            distance = fog.height + abs(fs_in.texture_coordinates.y);
        else
            distance = fog.height - fs_in.texture_coordinates.y;

        float fog_power = 1.0f - exp(-distance * fog.transition_power);
        fog_power = clamp(fog_power, 0.0f, 1.0f);
        result = mix(result, fog.color, fog_power);
    }

    return result;
}

void main()
{
    vec4 cube_texel = texture(color.cube_texture, fs_in.texture_coordinates);
    vec3 result_color = cube_texel.rgb;

    if (fog.enabled)
        result_color = calcFog(result_color);

    frag_color = vec4(result_color * color.filter_color * color.light_color,
        1.0f);
}