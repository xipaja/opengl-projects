#include "shader.h"

// Constructor
Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath) {
    // Retrieve vertex/fragment source code from file path
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertShaderFile;
    std::ifstream fragShaderFile;

    // Ensure ifstream objects can throw exceptions
    vertShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open files
        vertShaderFile.open(vertexFilePath);
        fragShaderFile.open(fragmentFilePath);
        std::stringstream vertShaderStream, fragShaderStream;
        // Read file's buffer contents into streams
        vertShaderStream << vertShaderFile.rdbuf();
        fragShaderStream << fragShaderFile.rdbuf();
        // Close file handlers
        vertShaderFile.close();
        fragShaderFile.close();
        // Convert stream into string
        vertexCode = vertShaderStream.str();
        fragmentCode = fragShaderStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "SHADER ERROR: File not successfully read" << std::endl;
    }

    const char* vertShaderCode = vertexCode.c_str();
    const char* fragShaderCode = fragmentCode.c_str();

    // Compile Shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertShaderCode, NULL);
    glCompileShader(vertex);
    // Print any compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "SHADER ERROR::VERTEX::Compilation failed\n" << infoLog << std::endl;
    }

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragShaderCode, NULL);
    glCompileShader(fragment);
    // Print any compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "SHADER ERROR::FRAGMENT::Compilation failed\n" << infoLog << std::endl;
    }

    // Shader program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    // Print any linking errors
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "SHADER ERROR::PROGRAM::Linking failed\n" << infoLog << std::endl;
    }

    // Delete shaders once they're linked
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(id);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 matrix) const {
    unsigned int location = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
    unsigned int location = glGetUniformLocation(id, name.c_str());
    glUniform3f(location, x, y, z);
}

void Shader::SetVec3(const std::string& name, glm::vec3 vec) const {
    unsigned int location = glGetUniformLocation(id, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vec));
}