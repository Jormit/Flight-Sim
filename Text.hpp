#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.hpp"

struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

class Text {
public:
    Text(int screenWidth, int screenHeight, const char *font);
    void renderText(std::string, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
    Shader shader;
    glm::mat4 projection;
    FT_Library ft;

    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;

};