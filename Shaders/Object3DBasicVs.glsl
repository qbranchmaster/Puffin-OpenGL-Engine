#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal_vector;
layout(location = 2) in vec2 texture_coord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

#define POINT_LIGHTS_COUNT 4

struct DirectionalLight
{
    bool enabled;
    vec3 color;
    vec3 direction;
};

struct PointLight
{
    bool enabled;
    vec3 position;
    vec3 color;    
    float linear_factor;
    float quadratic_factor;    
};

struct Matrices
{
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
    mat4 dir_light_pv_matrix;
    mat4 env_map_model_matrix;
};

struct Shadow
{
    bool enabled;
    float distance;
    float transition_distance;
    int map_size;
    int pcf_filter_count;
};

out VS_OUT
{
    float clip_height;

    vec3 normal_vector_VIEW;
    vec2 texture_coord_MODEL;
    vec3 view_position_TANGENT;
    vec3 position_WORLD;
    vec3 position_VIEW;
    vec3 position_TANGENT;
    vec3 point_light_position_VIEW[POINT_LIGHTS_COUNT];
    vec3 point_light_position_TANGENT[POINT_LIGHTS_COUNT];
    vec3 directional_light_direction_VIEW; 
    vec3 directional_light_direction_TANGENT;
    vec4 frag_pos_DIR_LIGHT;
} vs_out;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[POINT_LIGHTS_COUNT];
uniform int used_point_lights_count;

uniform Matrices matrices;
uniform Shadow shadow;

uniform vec4 clip_plane;

void main()
{
    mat3 normal_matrix = transpose(inverse(mat3(matrices.model_matrix)));

    vs_out.texture_coord_MODEL = texture_coord;
    vs_out.normal_vector_VIEW = normalize(mat3(normal_matrix) * normal_vector);

    vs_out.position_WORLD = vec3(matrices.model_matrix * vec4(position, 1.0f));
    vs_out.position_VIEW = vec3(matrices.view_matrix * 
        vec4(vs_out.position_WORLD, 1.0f));       

    vec3 camera_pos_WORLD = (inverse(matrices.view_matrix) * 
        vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;

    vec3 t = normalize(normal_matrix * tangent);
    vec3 n = normalize(normal_matrix * normal_vector);
    t = normalize(t - dot(t, n) * n);
    vec3 b = cross(n, t);
    
    mat3 tbn_matrix = transpose(mat3(t, b, n));

    vs_out.position_TANGENT = tbn_matrix * vs_out.position_WORLD;
    vs_out.view_position_TANGENT = tbn_matrix * camera_pos_WORLD;

    for (int i = 0; i < used_point_lights_count; i++)
    {
        vs_out.point_light_position_TANGENT[i] = tbn_matrix * 
            point_lights[i].position;
        vs_out.point_light_position_VIEW[i] = vec3(matrices.view_matrix * 
            vec4(point_lights[i].position, 1.0f));
    }

    vs_out.directional_light_direction_VIEW = normalize(vec3(
        matrices.view_matrix * vec4(directional_light.direction, 0.0f)));
    vs_out.directional_light_direction_TANGENT = normalize(tbn_matrix * 
        directional_light.direction);

    vs_out.clip_height = dot(vec4(vs_out.position_WORLD, 1.0f), clip_plane); 

    float distance = length(vs_out.position_VIEW);
    distance = distance - (shadow.distance - shadow.transition_distance);
    distance = distance / shadow.transition_distance;

    vs_out.frag_pos_DIR_LIGHT = matrices.dir_light_pv_matrix * 
        vec4(vs_out.position_WORLD, 1.0f);
    vs_out.frag_pos_DIR_LIGHT = 0.5f + 0.5f * vs_out.frag_pos_DIR_LIGHT;
    vs_out.frag_pos_DIR_LIGHT.w = clamp(1.0f - distance, 0.0f, 1.0f);

    gl_Position = matrices.projection_matrix * vec4(vs_out.position_VIEW, 1.0f);
}