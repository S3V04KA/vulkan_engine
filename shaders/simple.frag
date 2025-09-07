#version 450

layout(location = 0) out vec4 color;
layout(location = 0) in vec3 fragColor;

layout(push_constant) uniform Push{
    mat4 transform;
    vec3 color;
} push;

void main() {
    color = vec4(fragColor, 1.0);
}