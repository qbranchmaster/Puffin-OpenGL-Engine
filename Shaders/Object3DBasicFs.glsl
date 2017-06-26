#version 330 core

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

struct Fog
{
    bool enabled;
    vec3 color;
    float density;
};

struct Shadow
{
    bool enabled;
    float distance;
    float transition_distance;
    int map_size;
    int pcf_filter_count;
};

struct Material
{
    bool has_diffuse_texture;
    bool has_normalmap_texture;
    sampler2D diffuse_texture;
    sampler2D normalmap_texture;
    vec3 ka;
    vec3 kd;
    vec3 ks;
    int shininess;
    float reflectivity;
};

in GS_OUT
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
} fs_in;

out vec4 frag_color;

uniform bool lighting_enabled;
uniform DirectionalLight directional_light;
uniform PointLight point_lights[POINT_LIGHTS_COUNT];
uniform int used_point_lights_count;

uniform Matrices matrices;
uniform Fog fog;
uniform Shadow shadow;

uniform samplerCube point_shadow_map_1;
uniform samplerCube point_shadow_map_2;
uniform samplerCube point_shadow_map_3;
uniform samplerCube point_shadow_map_4;
uniform sampler2D shadow_map_texture;
uniform samplerCube env_map_texture;
uniform Material object_material;

float calcDirectionalShadow(vec4 frag_pos)
{
    float total_texels = (shadow.pcf_filter_count * 2.0f + 1.0f) * 
        (shadow.pcf_filter_count * 2.0f + 1.0f);

    float texel_size = 1.0f / shadow.map_size;
    float total = 0.0f;

    for (int x = -shadow.pcf_filter_count; x <= shadow.pcf_filter_count; x++)
    {
        for (int y = -shadow.pcf_filter_count; y <= shadow.pcf_filter_count; 
            y++)
        {
            float current_depth = frag_pos.z;
            if (current_depth > 1.0f)
                return 1.0f;

            float closest_depth = texture(shadow_map_texture, frag_pos.xy +
                vec2(x, y) * texel_size).r;
            if (current_depth  > closest_depth + 0.005f)
                total += 1.0f;
        }
    }

    total /= total_texels;
    float light_factor = 1.0f - (total * frag_pos.w * 1.0f);
    return light_factor;
}

vec3 calcDirectionalLight()
{
    vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
    vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

    // Get light direction
    vec3 light_direction = vec3(0.0f, 1.0f, 0.0f);
    if (object_material.has_normalmap_texture)
        light_direction = fs_in.directional_light_direction_TANGENT;
    else
        light_direction = fs_in.directional_light_direction_VIEW;

    // Get normal vector
    vec3 normal_vector = vec3(0.0f, 1.0f, 0.0f);
    if (object_material.has_normalmap_texture)
    {
        vec3 normal_vector_TANGENT = texture(object_material.normalmap_texture,  
            fs_in.texture_coord_MODEL).rgb;
        normal_vector_TANGENT = normalize(normal_vector_TANGENT * 2.0f - 1.0f);
        normal_vector = normal_vector_TANGENT;
    }
    else
    {
        normal_vector = fs_in.normal_vector_VIEW;
    }

    // Get view direction
    vec3 view_direction = vec3(0.0f, 1.0f, 0.0f);
    if (object_material.has_normalmap_texture)
    {
        vec3 view_direction_TANGENT = normalize(fs_in.view_position_TANGENT - 
            fs_in.position_TANGENT);
        view_direction = view_direction_TANGENT;
    }
    else
    {
        vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);
        view_direction = view_direction_VIEW;
    }

    // Calculate lighting
    // Ambient
    ambient = directional_light.color * object_material.ka;

    if (object_material.has_diffuse_texture)
        ambient = ambient * vec3(texture(object_material.diffuse_texture,
            fs_in.texture_coord_MODEL));

    // Diffuse
    float diffuse_power = max(dot(normal_vector, -light_direction), 0.0f);
    diffuse = directional_light.color * diffuse_power * object_material.kd;

    if (object_material.has_diffuse_texture)
    {
        vec4 texel = texture(object_material.diffuse_texture,
            fs_in.texture_coord_MODEL);
        if (texel.a < 0.2f)
            discard;

        diffuse = diffuse * vec3(texel);
    }

    // Specular
    vec3 reflected_ray = normalize(reflect(light_direction, normal_vector));
    float specular_power = pow(max(dot(reflected_ray, view_direction), 0.0f), 
        object_material.shininess);
    specular = directional_light.color * specular_power * object_material.ks;

    // Shadow
    float shadow_value = 1.0f;
    if (shadow.enabled)
        shadow_value = calcDirectionalShadow(fs_in.frag_pos_DIR_LIGHT);
    
    return (ambient + shadow_value * (diffuse + specular));
}

vec3 calcFog(vec3 input_color)
{
    float distance = length(fs_in.position_VIEW);

    float fog_power = 1.0f - exp(-fog.density * distance);
    fog_power = clamp(fog_power, 0.0f, 1.0f);
    vec3 result = mix(input_color, fog.color, fog_power);
    return result;
}

vec3 calcReflection(vec3 input_color)
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    // TODO: Fix it!
    vec3 view_direction = normalize(fs_in.position_WORLD);
    vec3 reflected = reflect(view_direction, fs_in.normal_vector_VIEW);
    reflected = normalize(vec3(matrices.env_map_model_matrix * 
        vec4(reflected, 0.0f)));

    vec3 skybox_frag = texture(env_map_texture, reflected).rgb;
    float ref = object_material.reflectivity / 10.0f;
    result = mix(input_color, skybox_frag * ref, 0.5f);

    return result;
}

float calcPointShadow(vec3 frag_pos, int light_index)
{
    vec3 frag_to_light = frag_pos - point_lights[light_index].position;

    float closest_depth = 0.0f;
    if (light_index == 0)
        closest_depth = texture(point_shadow_map_1, frag_to_light).r;
    else if (light_index == 1)
        closest_depth = texture(point_shadow_map_2, frag_to_light).r;
    else if (light_index == 2)
        closest_depth = texture(point_shadow_map_3, frag_to_light).r;
    else if (light_index == 3)
        closest_depth = texture(point_shadow_map_4, frag_to_light).r;
    closest_depth *= shadow.distance;

    float current_depth = length(frag_to_light);

    float bias = 0.05f;
    float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;

    return shadow;
}

vec3 calculatePointLight(int light_index)
{
    vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
    vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

    float vertex_dist = length(fs_in.point_light_position_VIEW[light_index] - 
        fs_in.position_VIEW);
    float attenuation = 1.0f / (1.0f + point_lights[light_index].linear_factor * 
        vertex_dist + point_lights[light_index].quadratic_factor * vertex_dist * 
        vertex_dist);

    // Get light direction
    vec3 light_direction = vec3(0.0f, 1.0f, 0.0f);
    if (object_material.has_normalmap_texture)
    {
        vec3 light_direction_TANGENT = normalize(
            fs_in.point_light_position_TANGENT[light_index] - 
            fs_in.position_TANGENT);
        light_direction = light_direction_TANGENT;
    }
    else
    {
        vec3 light_direction_VIEW = normalize(
            fs_in.point_light_position_VIEW[light_index] - fs_in.position_VIEW);
        light_direction = light_direction_VIEW;
    } 

    // Get normal vector
    vec3 normal_vector = vec3(0.0f, 1.0f, 0.0f);
    if (object_material.has_normalmap_texture)
    {
        vec3 normal_vector_TANGENT = texture(object_material.normalmap_texture,  
            fs_in.texture_coord_MODEL).rgb;
        normal_vector_TANGENT = normalize(normal_vector_TANGENT * 2.0f - 1.0f);
        normal_vector = normal_vector_TANGENT;
    }
    else
    {
        normal_vector = fs_in.normal_vector_VIEW;
    }

    // Get view direction
    vec3 view_direction = vec3(0.0f, 1.0f, 0.0f);
    if (object_material.has_normalmap_texture)
    {
        vec3 view_direction_TANGENT = normalize(fs_in.view_position_TANGENT - 
            fs_in.position_TANGENT);
        view_direction = view_direction_TANGENT;
    }
    else
    {
        vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);
        view_direction = view_direction_VIEW;
    }

    // Calculate lighting
    // Ambient
    ambient = point_lights[light_index].color * attenuation * 
        object_material.ka;

    if (object_material.has_diffuse_texture)
        ambient = ambient * vec3(texture(object_material.diffuse_texture, 
            fs_in.texture_coord_MODEL));

    // Diffuse
    float diffuse_power = max(dot(normal_vector, light_direction), 0.0f);
    diffuse = point_lights[light_index].color * diffuse_power * attenuation * 
        object_material.kd;

    if (object_material.has_diffuse_texture)
        diffuse = diffuse * vec3(texture(object_material.diffuse_texture, 
            fs_in.texture_coord_MODEL));

    // Specular
    vec3 reflected_ray = normalize(reflect(-light_direction, normal_vector));
    float specular_power = pow(max(dot(reflected_ray, view_direction), 0.0f), 
        object_material.shininess);
    specular = point_lights[light_index].color * specular_power * 
        object_material.ks * attenuation;

    // Shadow
    float shadow_value = 1.0f;
    if (shadow.enabled)
        shadow_value = 1.0f - calcPointShadow(fs_in.position_WORLD, 
            light_index);

    return (ambient + shadow_value * (diffuse + specular));
}

void main()
{
    vec3 result_color = vec3(0.0f, 0.0f, 0.0f);

    if (lighting_enabled)
    {
        if (directional_light.enabled)
            result_color = calcDirectionalLight();

        for (int i = 0; i < used_point_lights_count; i++)
        {
            if (point_lights[i].enabled)
                result_color += calculatePointLight(i);
        }
    }
    else
    {
        if (object_material.has_diffuse_texture)
            result_color = vec3(texture(object_material.diffuse_texture,
                fs_in.texture_coord_MODEL));
        else
            result_color = object_material.kd;
    }

    if (object_material.reflectivity > 0.0f)
        result_color = calcReflection(result_color);

    if (fog.enabled)
        result_color = calcFog(result_color);

    frag_color = vec4(result_color, 1.0f);
}