#version 330 core

in vec4 frag_pos;

uniform vec3 light_position;
uniform float shadow_distance;

void main()
{
    float light_distance = length(frag_pos.xyz - light_position);
    light_distance = light_distance / shadow_distance;
    gl_FragDepth = light_distance;
}