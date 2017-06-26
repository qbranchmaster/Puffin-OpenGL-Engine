#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

#define POINT_LIGHTS_COUNT 4

in VS_OUT
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
} gs_in[];

out GS_OUT
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
} gs_out;

uniform int used_point_lights_count;

vec3 getNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
} 

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 0.0f;
    vec3 direction = normal * magnitude; 
    return position + vec4(direction, 0.0f);
}

void main()
{
    vec3 normal = getNormal();

    gs_out.texture_coord_MODEL = gs_in[0].texture_coord_MODEL;
    gs_out.position_VIEW = gs_in[0].position_VIEW;
    gs_out.normal_vector_VIEW = gs_in[0].normal_vector_VIEW;
    gs_out.frag_pos_DIR_LIGHT = gs_in[0].frag_pos_DIR_LIGHT;
    gs_out.position_WORLD = gs_in[0].position_WORLD;
    gs_out.position_TANGENT = gs_in[0].position_TANGENT;
    gs_out.view_position_TANGENT = gs_in[0].view_position_TANGENT;
    gs_out.directional_light_direction_TANGENT = 
        gs_in[0].directional_light_direction_TANGENT;
    gs_out.directional_light_direction_VIEW = 
        gs_in[0].directional_light_direction_VIEW;

    for (int i = 0; i < used_point_lights_count; i++)
    {
        gs_out.point_light_position_TANGENT[i] = gs_in[0].
            point_light_position_TANGENT[i];
        gs_out.point_light_position_VIEW[i] = gs_in[0].
            point_light_position_VIEW[i];
    }    
    
    gl_Position = explode(gl_in[0].gl_Position, normal);
    gl_ClipDistance[0] = gs_in[0].clip_height;

    EmitVertex();

    gs_out.texture_coord_MODEL = gs_in[1].texture_coord_MODEL;
    gs_out.position_VIEW = gs_in[1].position_VIEW;
    gs_out.normal_vector_VIEW = gs_in[1].normal_vector_VIEW;
    gs_out.frag_pos_DIR_LIGHT = gs_in[1].frag_pos_DIR_LIGHT;
    gs_out.position_WORLD = gs_in[1].position_WORLD;
    gs_out.position_TANGENT = gs_in[1].position_TANGENT;
    gs_out.view_position_TANGENT = gs_in[1].view_position_TANGENT;
    gs_out.directional_light_direction_TANGENT = 
        gs_in[1].directional_light_direction_TANGENT;
    gs_out.directional_light_direction_VIEW = 
        gs_in[1].directional_light_direction_VIEW;

    for (int i = 0; i < used_point_lights_count; i++)
    {
        gs_out.point_light_position_TANGENT[i] = gs_in[1].
            point_light_position_TANGENT[i];
        gs_out.point_light_position_VIEW[i] = gs_in[1].
            point_light_position_VIEW[i];
    }  

    gl_Position = explode(gl_in[1].gl_Position, normal);
    gl_ClipDistance[0] = gs_in[1].clip_height;

    EmitVertex();

    gs_out.texture_coord_MODEL = gs_in[2].texture_coord_MODEL;
    gs_out.position_VIEW = gs_in[2].position_VIEW;
    gs_out.normal_vector_VIEW = gs_in[2].normal_vector_VIEW;
    gs_out.frag_pos_DIR_LIGHT = gs_in[2].frag_pos_DIR_LIGHT;
    gs_out.position_WORLD = gs_in[2].position_WORLD;
    gs_out.position_TANGENT = gs_in[2].position_TANGENT;
    gs_out.view_position_TANGENT = gs_in[2].view_position_TANGENT;
    gs_out.directional_light_direction_TANGENT = 
        gs_in[2].directional_light_direction_TANGENT;
    gs_out.directional_light_direction_VIEW = 
        gs_in[2].directional_light_direction_VIEW;

    for (int i = 0; i < used_point_lights_count; i++)
    {
        gs_out.point_light_position_TANGENT[i] = gs_in[2].
            point_light_position_TANGENT[i];
        gs_out.point_light_position_VIEW[i] = gs_in[2].
            point_light_position_VIEW[i];
    }  

    gl_Position = explode(gl_in[2].gl_Position, normal);
    gl_ClipDistance[0] = gs_in[2].clip_height;
    
    EmitVertex();

    EndPrimitive();
}