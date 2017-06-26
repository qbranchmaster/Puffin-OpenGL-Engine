#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coords;

out vec2 texture_coords;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

uniform int texture_row;
uniform int texture_col;
uniform int atlas_size;

void main()
{
    mat4 model_view_matrix = view_matrix * model_matrix;
    gl_Position = projection_matrix * model_view_matrix * vec4(position, 1.0f);

    // Texture
    float val = 1.0f / atlas_size;
    
    if (position.x < 0.0f && position.y < 0.0f) // Bottom left vertex
    {
        float u = val * texture_col;
        float v = (texture_row + 1) * val;

        texture_coords = vec2(u, v);
    }
    else if (position.x < 0.0f && position.y > 0.0f) // Top left vertex
    {
        float u = val * texture_col;
        float v = (texture_row) * val;

        texture_coords = vec2(u, v);
    }
    else if (position.x > 0.0f && position.y < 0.0f) // Bottom right vertex
    {
        float u = val * (texture_col + 1);
        float v = (texture_row + 1) * val;

        texture_coords = vec2(u, v);
    }
    else if (position.x > 0.0f && position.y > 0.0f) // Top right vertex
    {
        float u = val * (texture_col + 1);
        float v = (texture_row) * val;

        texture_coords = vec2(u, v);
    }
}