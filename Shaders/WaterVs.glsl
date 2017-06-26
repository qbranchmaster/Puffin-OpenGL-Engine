#version 330 core

#define POINT_LIGHTS_COUNT 4

layout(location = 0) in vec3 position;

struct Matrices
{
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
};

struct PointLight
{
    bool enabled;
    vec3 position;
    vec3 color;    
    float linear_factor;
    float quadratic_factor; 
};

out VS_OUT
{
    vec2 texture_coords;
    vec4 clip_space;
    vec3 to_camera_vector;
    vec3 position_VIEW;
    vec3 point_light_position_VIEW[POINT_LIGHTS_COUNT];
} vs_out;

uniform Matrices matrices;
uniform int texture_tiling;
uniform vec3 camera_position;

uniform PointLight point_lights[POINT_LIGHTS_COUNT];
uniform int used_point_lights_count;

void main()
{
    vec3 world_pos = vec3(matrices.model_matrix * vec4(position.x, position.y, 
        position.z, 1.0f));

    vs_out.texture_coords = vec2(position.x / 2.0f + 0.5f, 
        position.z / 2.0f + 0.5f) * texture_tiling;
    vs_out.to_camera_vector = camera_position - world_pos;

    vs_out.position_VIEW =  vec3(matrices.view_matrix * vec4(world_pos, 1.0f));
    vs_out.clip_space = matrices.projection_matrix * 
        vec4(vs_out.position_VIEW, 1.0f);

    for (int i = 0; i < used_point_lights_count; i++)
    {
        vs_out.point_light_position_VIEW[i] = vec3(matrices.view_matrix * 
            vec4(point_lights[i].position, 1.0f));
    }

    gl_Position = vs_out.clip_space;
}