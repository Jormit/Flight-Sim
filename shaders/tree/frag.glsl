#version 330 core
in vec2 texCoord;
uniform sampler2D objTexture;
out vec4 FragColor;

void main(){
    vec4 texColor = texture2D(objTexture, texCoord);
    if(texColor.a < 0.5) discard; // For clarity on alpha channel.
    FragColor = texColor;
}