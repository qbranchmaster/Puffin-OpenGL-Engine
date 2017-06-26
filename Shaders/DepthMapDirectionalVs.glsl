#version 330 core

layout(location = 0) in vec3 position;

struct Matrices
{
    mat4 model_matrix;
    mat4 light_space_matrix;
};

uniform Matrices matrices;

void main()
{
    gl_Position = matrices.light_space_matrix * matrices.model_matrix * 
        vec4(position, 1.0f);
}