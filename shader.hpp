#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shader class for loading shader from file and compiling.
class Shader
{
public:
    Shader (const char *vectFile, const char *fragFile);
    void use();
    void setInt(const char *name, int a);
    void setMat4(const char *name, glm::mat4 matrix);
    void setVec4 (const char *name, glm::vec4 vector);
    unsigned int shaderProgram;
private:
    unsigned int vertexShader, fragShader;
    int readFile(const char* filename, std::vector<char>& buffer);
    void compile(const char* Path, GLenum type, unsigned int *handle);
    void linkShaders();
};

