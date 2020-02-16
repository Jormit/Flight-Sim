#include "texture.hpp"
using namespace std;

Texture::Texture(const char* file) {
    texture.type = GL_TEXTURE_2D;
    glGenTextures(1, &(texture.id));
    glBindTexture(texture.type, texture.id);
    // Set wrapping parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set filtering parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Load texture image.
    int tex_width, tex_height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file, &tex_width, &tex_height, &nrChannels, STBI_default);

    // If texture loaded successfuly then generate mipmaps.
    if (data) {
        if (nrChannels == 3) {
            glTexImage2D(texture.type, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4) {
            glTexImage2D(texture.type, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            cerr << "ERROR: " << "Invalid number of channels in " << file << endl;
            exit(1);
        }
        glGenerateMipmap(texture.type);
    }
    else {
        cerr << "ERROR: " << "Could not load texture: " << file << endl;
        exit(1);
    }
    stbi_image_free(data);

    cout << "Loaded texture: " << file << ", w = " << tex_width << ", h = " <<
    tex_height << ", channels = " << nrChannels << endl;
}

void Texture::bindTexture(){
    glBindTexture(texture.type, texture.id);
}

unsigned int Texture::getId(){
    return texture.id;
}
