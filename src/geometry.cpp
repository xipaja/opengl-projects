#include "geometry.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Geometry::Geometry(const char* vertPath, const char* fragPath) :
_customShader(vertPath, fragPath) {}

void Geometry::_SetUpBuffers(const std::vector<float>& vertexData) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    // Bind VAO, then bind and set VBO, then config attributes
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
}

void Geometry::_SetUpAttributes() {}

void Geometry::_SetUpTexture(const char* fileName) {
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture
    _textureImageData = stbi_load(fileName, &_textureWidth, &_textureHeight, &_textureNumChannels, 0);

    // Generate texture
    if (_textureImageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _textureWidth, _textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _textureImageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "ERROR::TEXTURE::Failed to load texture" << std::endl;
    }

    stbi_image_free(_textureImageData);
}

Shader& Geometry::GetShader() {
    return _customShader;
}

void Geometry::BindTexture() {
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void Geometry::BindVertexArray() {
    glBindVertexArray(_VAO);
}

void Geometry::SetUpProjectionMatrix(float fov, float aspectRatio) {
    float zNear = 0.1f;
    float zFar = 100.0f;
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    _customShader.SetMat4("projection", projectionMatrix);
}

void Geometry::SetUpCamViewTransform(glm::mat4 viewMatrix) {}

void Geometry::SetUpTransformations() {}

void Geometry::Draw() {}

Geometry::~Geometry() {
    // Deallocate resources
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}