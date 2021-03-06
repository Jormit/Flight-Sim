#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Map.hpp"
#include "Text.hpp"

using namespace std;

#define WIDTH 980
#define HEIGHT 620

GLFWwindow* initGL();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback (GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window, float deltaTime);

float deltaTime;
float lastFrame;

int main() {
    GLFWwindow *window = initGL();
    Map map(100, 0.06, 4, 20, 2, 10);
    Text text(WIDTH, HEIGHT, "../assets/fonts/monospace.ttf");

    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time.
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        // Clear screen and z-buffer.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-10.0f, 50.0f, 10.0f),
                    glm::vec3(50.0, 0.0, -50.0),
                    glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f),
                (float) WIDTH / (float) HEIGHT, 0.1f, 1000.0f);

        map.draw(viewMatrix, projMatrix, glm::vec4(0.0, 100.0, 0.0, 1.0));

        text.renderText("Hello", 25.0f, 25.0f, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

        // Flip the buffers and check for events.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

GLFWwindow* initGL() {
    GLFWwindow *window = NULL;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "FlightSim", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return window;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return window;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback (GLFWwindow* window, double xpos, double ypos) {

}

void processInput(GLFWwindow *window, float deltaTime){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}