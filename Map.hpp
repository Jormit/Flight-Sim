#pragma once
#include <vector>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "shader.hpp"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture;
};

class Map {
public:
    Map(int size);
    void draw();
private:
    unsigned int VAO, VBO, EBO;
    Vertex getVertex(const glm::vec2 position, float freq, int depth, int heightScale);
    Shader shader;
    int size;
};

// Perlin Noise functions.
int noise2(int x, int y);
float lin_inter(float x, float y, float s);
float smooth_inter(float x, float y, float s);
float noise2d(float x, float y);
float perlin2d(float x, float y, float freq, int depth);

