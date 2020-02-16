#include "shader.hpp"
using namespace std;

Shader::Shader (const char *vectFile, const char *fragFile)
{
     // Compile the shaders.
    compile(vectFile, GL_VERTEX_SHADER, &vertexShader);
    compile(fragFile, GL_FRAGMENT_SHADER, &fragShader);

    // Link shaders into main shader program and cleanup.
    linkShaders();

    cout << "Shaders: " << vectFile << " & " << fragFile << " compiled successfully!" << endl;
}

void Shader::use ()
{
    glUseProgram(shaderProgram);
}

void Shader::compile(const char* Path, GLenum type, unsigned int *handle)
{
    // Read shader from path.
    vector<char> fileContent;

    readFile(Path, fileContent);
    const char* shaderText (&fileContent[0]);

    // Create and compile the shader.
    *handle = glCreateShader(type);

    glShaderSource(*handle, 1, (const GLchar**)&shaderText, NULL);
    glCompileShader(*handle);

    // Check for any errors.
    int  success;
    char infoLog[512];

    if (type == GL_VERTEX_SHADER){
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    } else if (type == GL_FRAGMENT_SHADER) {
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    }

    if(!success)
    {
        if (type == GL_VERTEX_SHADER){
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            cerr << "ERROR: " << "Vert shader could not compile:" << endl;
            cerr << infoLog << endl;
        } else if (type == GL_FRAGMENT_SHADER) {
            glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
            cerr << "ERROR: "<< "Frag shader could not compile:" << endl;
            cerr << infoLog << endl;
        }
        exit(1);
    }
}

void Shader::linkShaders()
{
    // Attatch the compiled shaders to the main program.
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // Check for any errors.
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "ERROR: " << "Could not link shaders: " << infoLog << endl;
        exit(1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

int Shader::readFile(const char* filename, std::vector<char>& buffer)
{
    ifstream file(filename, ios_base::binary);
    if (file) {
        file.seekg(0, ios_base::end);
        streamsize size = file.tellg();
        if (size > 0) {
            file.seekg(0, ios_base::beg);
            buffer.resize(static_cast<size_t>(size));
            file.read(&buffer[0], size);
        }
        buffer.push_back('\0');
    } else {
        cerr << "ERROR: " << filename << " does not exist." << endl;
        exit(1);
    }
}

void Shader::setMat4(const char *name, glm::mat4 matrix) {
    unsigned int loc = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec4 (const char *name, glm::vec4 vector) {
    unsigned int loc = glGetUniformLocation(shaderProgram, name);
    glUniform4fv(loc, 1, glm::value_ptr(vector));
}

void Shader::setInt(const char *name, int a)
{
    glUniform1i(glGetUniformLocation(shaderProgram, name), a);
}

unsigned int Shader::getId(){
   return shaderProgram;
}
