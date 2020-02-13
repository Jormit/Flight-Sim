#version 330 core
out vec4 FragColor;

in vec4 lightPosition;
in vec3 normal;
in vec4 position;

void main(){
    vec3 n = normalize(normal);
    vec3 l = normalize(lightPosition.xyz - position.xyz);

    float diffus = 0.7 * max(0.0,dot(n,l));
    FragColor = diffus * vec4(1.0, 1.0, 1.0, 1.0);
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}