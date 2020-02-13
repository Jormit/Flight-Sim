#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inText;

uniform mat4 view;
uniform mat4 projection;

out vec4 lightPosition;
out vec3 normal;
out vec4 position;

void main() {
    lightPosition = view * vec4(0.0, 50.0, 0.0, 1.0);
    normal = vec3(view * vec4(inNorm,0.0));
    position = view * vec4(inPos, 1.0);

    gl_Position =  projection * position;
}