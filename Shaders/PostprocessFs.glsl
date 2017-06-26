#version 330 core

struct Color
{
    sampler2D screen_texture;
    vec3 tint_color; 
    int effect;
    float kernel_size;   
};

in VS_OUT
{
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform Color color;

vec3 noEffect()
{
    return vec3(texture(color.screen_texture, fs_in.tex_coord));
}

vec3 negative()
{
    return vec3(1.0f - texture(color.screen_texture, fs_in.tex_coord));
}

vec3 grayScale()
{
    vec3 texel_color = noEffect();
    float average = (texel_color.r + texel_color.g + texel_color.b) / 3.0f;
    return vec3(average, average, average);
}

vec3 tintColor()
{
    vec3 texel_color = noEffect();
    return texel_color * color.tint_color;
}

vec3 calcPostprocess(vec2 offsets[9], float kernel[9])
{
    vec3 sample_tex[9];
    for (int i = 0; i < 9; i++)
        sample_tex[i] = vec3(texture(color.screen_texture, fs_in.tex_coord.st +
            offsets[i]));

    vec3 output = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 9; i++)
        output += sample_tex[i] * kernel[i];

    return output;
}

vec3 sharpen(vec2 offsets[9])
{
    float kernel[9] = float[](
        -1.0f, -1.0f, -1.0f,
        -1.0f,  9.0f, -1.0f,
        -1.0f, -1.0f, -1.0f);

    return calcPostprocess(offsets, kernel);
}

vec3 blur(vec2 offsets[9])
{
    float kernel[9] = float[](
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    );

    return calcPostprocess(offsets, kernel);
}

vec3 edge(vec2 offsets[9])
{
    float kernel[9] = float[](
        1.0f,  1.0f, 1.0f,
        1.0f, -8.0f, 1.0f,
        1.0f,  1.0f, 1.0f);

    return calcPostprocess(offsets, kernel);
}

void main()
{
    float offset = 1.0f / color.kernel_size;
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0.0f, offset),
        vec2(offset, offset),
        vec2(-offset, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(offset, 0.0f),
        vec2(-offset, -offset),
        vec2(0.0f, -offset),
        vec2(offset, -offset));

    vec3 result_color = vec3(0.0f, 0.0f, 0.0f);

    switch (color.effect)
    {
    case 0: // None
        result_color = noEffect();
        break;
    case 1: // Negative
        result_color = negative();
        break;
    case 2: // Grayscale
        result_color = grayScale();
        break;
    case 3: // Sharpen
        result_color = sharpen(offsets);
        break;
    case 4: // Blur
        result_color = blur(offsets);
        break;
    case 5: // Edge
        result_color = edge(offsets);
        break;
    case 6: // Tint
        result_color = tintColor();
        break;
    }

    frag_color = vec4(result_color, 1.0f);
}