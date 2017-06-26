#version 330 core

layout (location = 0) in vec3 position;

struct Matrices
{
    mat4 model_matrix;
};

uniform Matrices matrices;

void main()
{
    gl_Position = matrices.model_matrix * vec4(position, 1.0);
}