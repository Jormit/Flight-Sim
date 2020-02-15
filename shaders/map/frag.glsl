#version 330 core
in vec4 lightPosition;
in vec3 normal;
in vec4 position;
in vec2 texCoord;
in vec4 splat;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

out vec4 FragColor;

void main(){
    vec3 n = normalize(normal);
    vec3 l = normalize(lightPosition.xyz);
    float diffus = max(0.0,dot(n,l));

    vec4 texel1 = texture2D(texture1, texCoord);
    vec4 texel2 = texture2D(texture2, texCoord);
    vec4 texel3 = texture2D(texture3, texCoord);
    vec4 texel4 = texture2D(texture4, texCoord);

    FragColor = diffus * 2.0 * (texel1 * splat.r + texel2 * splat.g + texel3 * splat.b + texel4 * splat.a);
}