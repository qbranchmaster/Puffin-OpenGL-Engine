#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

struct Matrix
{
    mat4 mat;
};

uniform Matrix shadow_matrices[6];

out vec4 frag_pos; 

void main()
{
    for (int face = 0; face < 6; face++)
    {
        gl_Layer = face; 
        for (int i = 0; i < 3; i++)
        {
            frag_pos = gl_in[i].gl_Position;
            gl_Position = shadow_matrices[face].mat * frag_pos;
            EmitVertex();
        }    

        EndPrimitive();
    }
}