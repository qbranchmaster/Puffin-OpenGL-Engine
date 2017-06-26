#version 330 core

#define POINT_LIGHTS_COUNT 4

struct Matrices
{
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
};

struct Fog
{
    bool enabled;
    vec3 color;
    float density;
};

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

in VS_OUT
{
    vec2 texture_coords;
    vec4 clip_space;
    vec3 to_camera_vector;
    vec3 position_VIEW;
    vec3 point_light_position_VIEW[POINT_LIGHTS_COUNT];
} fs_in;

out vec4 frag_color;

uniform Matrices matrices;
uniform Fog fog;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D dudv_map;
uniform sampler2D normal_map;
uniform sampler2D depth_map;

uniform float clip_near;
uniform float clip_far;
uniform float move_factor;

uniform vec3 water_color;
uniform float wave_strenght;
uniform int shininess;

uniform DirectionalLight directional_light;
uniform PointLight point_lights[POINT_LIGHTS_COUNT];
uniform int used_point_lights_count;

vec3 calcFog(vec3 input_color)
{
    float distance = length(fs_in.position_VIEW);

    float fog_power = 1.0f - exp(-fog.density * distance);
    fog_power = clamp(fog_power, 0.0f, 1.0f);
    vec3 result = mix(input_color, fog.color, fog_power);
    return result;
}

vec3 calcDirectionalLight(vec3 input_color, vec3 normal_vector)
{
    vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
    vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

    vec3 light_direction_VIEW = normalize(vec3(matrices.view_matrix * 
        vec4(directional_light.direction, 0.0f)));
    
    // Ambient
    ambient = directional_light.color * 0.4f;

    // Diffuse
    float diffuse_power = max(dot(normal_vector, -light_direction_VIEW), 0.0f);
    diffuse = directional_light.color * diffuse_power * 0.6f;

    // Specular
    vec3 reflected_ray = normalize(reflect(light_direction_VIEW, 
        normal_vector));
    vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);

    float specular_power = pow(max(dot(reflected_ray, view_direction_VIEW),
        0.0f), shininess);
    specular = directional_light.color * specular_power * 0.8f;

    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(vec3 input_color, vec3 normal_vector, int light_index)
{
    vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
    vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

    float vertex_dist = length(fs_in.point_light_position_VIEW[light_index] - 
        fs_in.position_VIEW);
    float attenuation = 1.0f / (1.0f + point_lights[light_index].linear_factor * 
        vertex_dist + point_lights[light_index].quadratic_factor * vertex_dist * 
        vertex_dist);

    vec3 light_direction_VIEW = normalize(
        fs_in.point_light_position_VIEW[light_index] - fs_in.position_VIEW);
    vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);

    // Ambient
    ambient = point_lights[light_index].color * attenuation * 0.4f;

    // Diffuse
    float diffuse_power = max(dot(normal_vector, light_direction_VIEW), 0.0f);
    diffuse = point_lights[light_index].color * diffuse_power * attenuation * 
        0.6f;

    // Specular
    vec3 reflected_ray = normalize(reflect(-light_direction_VIEW, 
        normal_vector));
    float specular_power = pow(max(dot(reflected_ray, view_direction_VIEW), 
        0.0f), shininess);
    specular = point_lights[light_index].color * specular_power * 
        0.8f * attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    vec2 ndc = (fs_in.clip_space.xy / fs_in.clip_space.w) / 2.0f + 0.5f;
    vec2 refract_tex_coords = ndc;
    vec2 reflect_tex_coords = vec2(ndc.x, -ndc.y);

    // Soft edges
    float depth = texture(depth_map, refract_tex_coords).r;
    // Linearize depth values
    float floor_distance = 2.0f * clip_near * clip_far / 
        (clip_far + clip_near  - (2.0f * depth - 1.0f) * 
        (clip_far - clip_near));

    depth = gl_FragCoord.z;
    float water_distance = 2.0f * clip_near * clip_far / 
        (clip_far + clip_near  - (2.0f * depth - 1.0f) * 
        (clip_far - clip_near));

    // Multiply water depth by some value. Increasing this value causes bigger
    // depth differences between water edges and objects underwater.
    float water_depth = (floor_distance - water_distance) * 2.0f;

    vec2 distorted_tex_coords = texture(dudv_map, vec2(fs_in.texture_coords.x +
        move_factor, fs_in.texture_coords.y)).rg * 0.1f;
    distorted_tex_coords = fs_in.texture_coords + vec2(distorted_tex_coords.x,
        distorted_tex_coords.y + move_factor);
    
    vec2 total_distortion = (texture(dudv_map, distorted_tex_coords).rg * 
        2.0f - 1.0f) * wave_strenght * clamp(water_depth / 2.5f, 0.0f, 1.0f);

    refract_tex_coords += total_distortion;
    refract_tex_coords = clamp(refract_tex_coords, 0.001f, 0.999f);

    reflect_tex_coords += total_distortion;
    reflect_tex_coords.x = clamp(reflect_tex_coords.x, 0.001f, 0.999f);
    reflect_tex_coords.y = clamp(reflect_tex_coords.y, -0.999f, 0.001f);

    vec4 reflection_color = texture(reflection_texture, reflect_tex_coords);
    vec4 refraction_color = texture(refraction_texture, refract_tex_coords);

    // Calculate normal vectors
    vec4 normal_map_color = texture(normal_map, distorted_tex_coords);
    vec3 normal = vec3(normal_map_color.r * 2.0f - 1.0f, normal_map_color.b, 
        normal_map_color.g * 2.0f - 1.0f);
    normal = vec3(matrices.view_matrix * vec4(normal, 0.0f));
    normal = normalize(normal);

    // Fresnel effect
    vec3 view_vector = normalize(fs_in.to_camera_vector);
    float fresnel_factor = dot(view_vector, vec3(0.0f, 1.0f, 0.0f));

    frag_color = mix(reflection_color, refraction_color, fresnel_factor);
    frag_color = mix(frag_color, vec4(water_color, 1.0f), 0.2f);

    vec3 light_int = vec3(0.0f, 0.0f, 0.0f);
    if (directional_light.enabled)
        light_int = light_int + calcDirectionalLight(frag_color.rgb, normal);
    
    for (int i = 0; i < used_point_lights_count; i++)
    {
        if (point_lights[i].enabled)
            light_int = light_int + calculatePointLight(frag_color.rgb, 
            normal, i);
    }

    frag_color = frag_color * vec4(light_int, 1.0f);

    if (fog.enabled)
        frag_color = vec4(calcFog(frag_color.rgb), 1.0f);
}