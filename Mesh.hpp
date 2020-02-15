#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <tiny_obj_loader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "shader.hpp"
#include "Texture.hpp"

struct buffer {
    unsigned int VAO;
    unsigned int VBO;
    long int size;
};

class Mesh {
public:
    Mesh(const std::string& filename, const std::string& base, const char *vectFile, const char *fragFile);
    Mesh(const std::string& filename, const std::string& base, std::vector<std::string> df_textures, const char *vectFile, const char *fragFile);
    void draw(glm::mat4 viewMat, glm::mat4 projMat);
    void setModelTransformation(glm::mat4 matrix);
private:
    void setupBuffers();
    void setupTextures();
    void loadFile(const std::string& filename, const std::string& base);


    Shader meshShader;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<std::string> df_textures; //
    std::vector<Texture> textures;
    std::vector<buffer> glBuffers;

    glm::mat4 modelMatrix;

    unsigned int numShapes;
};