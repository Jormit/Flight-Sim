#pragma once
#include <vector>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture;
  glm::vec4 splat;
};

class Map {
public:
    Map(int size, float freq, int depth, int heightScale, int seed, int tileSize);
    void draw(glm::mat4 viewMat, glm::mat4 projMat, glm::vec4 lightPos);
private:
    unsigned int VAO, VBO, EBO;
    Vertex getVertex(const glm::vec2 position, float freq, int depth, int heightScale, int tileSize, int seed);
    Shader shader;

    // Splatted textures.
    Texture texture1;
    Texture texture2;
    Texture texture3;
    Texture texture4;
    int size;

    // Tree.
    Mesh tree1;
};

// Perlin Noise functions.
int noise2(int x, int y);
float lin_inter(float x, float y, float s);
float smooth_inter(float x, float y, float s);
float noise2d(float x, float y);
float perlin2d(float x, float y, float freq, int depth, int seed);

