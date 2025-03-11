#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float outlineScale;

void main() {
    vec3 scaledPos = aPos + aNormal * outlineScale;
    gl_Position = projection * view * model * vec4(scaledPos, 1.0);
} 