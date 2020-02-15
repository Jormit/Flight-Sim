#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

struct textureInternal
{
    unsigned int id;
    int type;
};

class Texture {
public:
    Texture(const char* file);
    void bindTexture();
    unsigned int getId();
private:
    textureInternal texture;
};