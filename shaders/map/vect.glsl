#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inText;
layout (location = 3) in vec4 inSplat;

uniform mat4 view;
uniform mat4 projection;
uniform vec4 inLightPos;

out vec4 lightPosition;
out vec3 normal;
out vec4 position;
out vec2 texCoord;
out vec4 splat;

void main() {
    lightPosition = view * vec4(0.0, 100.0, 0.0, 1.0);
    normal = vec3(view * vec4(inNorm,0.0));
    position = view * vec4(inPos, 1.0);

    texCoord = inText;
    splat = inSplat;

    gl_Position =  projection * position;
}