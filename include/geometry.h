#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "window.h"
#include "shader.h"

class Geometry {
    public:
        Geometry(const char* vertPath, const char* fragPath);
        ~Geometry();
        Shader& GetShader();
        void BindTexture();
        void BindVertexArray();
        void SetUpProjectionMatrix(float fov, float aspectRatio);
        virtual void SetUpCamViewTransform(glm::mat4 viewMatrix);
        virtual void SetUpTransformations();
        virtual void Draw() = 0;

    protected:
        unsigned int _VBO, _VAO;
        Shader _customShader;

        void _SetUpBuffers(const std::vector<float>& vertexData);
        virtual void _SetUpAttributes() = 0;
        virtual void _SetUpTexture(const char* fileName);
    
    private:
        unsigned int _texture;
        int _textureWidth, _textureHeight, _textureNumChannels;
        unsigned char* _textureImageData;
};

#endif