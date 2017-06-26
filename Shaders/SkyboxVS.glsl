#version 330 core

layout(location = 0) in vec3 position;

struct Matrices
{
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
};

out VS_OUT
{
    vec3 texture_coordinates;
} vs_out;

uniform Matrices matrices;

void main()
{
    vs_out.texture_coordinates = position;

    gl_Position = matrices.projection_matrix * matrices.view_matrix * 
        matrices.model_matrix * vec4(position, 1.0f);
}