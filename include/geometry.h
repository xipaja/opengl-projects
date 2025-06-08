#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "window.h"
#include "vertices.h"
#include "shader.h"

class Geometry {
    public:
        Geometry();
        ~Geometry();
        Shader& GetShader();
        void BindTexture();
        void BindVertexArray();
        virtual void SetUpProjectionMatrix(float fov, float aspectRatio);
        virtual void SetUpCamViewTransform(glm::mat4 viewMatrix);
        virtual void SetUpTransformations();
        virtual void Draw();

    protected:
        unsigned int _VBO, _VAO;
        Shader _customShader;

        void _SetUpBuffers();
        virtual void _SetUpAttributes() = 0;
        virtual void _SetUpTexture();
    
    private:
        unsigned int _texture;
        int _textureWidth, _textureHeight, _textureNumChannels;
        unsigned char* _textureImageData;
};

Geometry::Geometry() :
_customShader("shaders/cube/vertex.vs", "shaders/cube/fragment.fs") {}

void Geometry::_SetUpBuffers() {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    // Bind VAO, then bind and set VBO, then config attributes
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_EXAMPLE_VERTICES), CUBE_EXAMPLE_VERTICES, GL_STATIC_DRAW);
}

void Geometry::_SetUpAttributes() {}

void Geometry::_SetUpTexture() {
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture
    _textureImageData = stbi_load("fun_pattern.jpg", &_textureWidth, &_textureHeight, &_textureNumChannels, 0);

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

void Geometry::SetUpProjectionMatrix(float fov, float aspectRatio) {}

void Geometry::SetUpCamViewTransform(glm::mat4 viewMatrix) {}

void Geometry::SetUpTransformations() {}

void Geometry::Draw() {}

Geometry::~Geometry() {
    // Deallocate resources
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

#endif