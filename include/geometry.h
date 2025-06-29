#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
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
        virtual void SetModelMatrix(glm::mat4 modelMatrix);
        virtual void Draw() = 0;

    protected:
        unsigned int _VBO, _VAO;
        Shader _customShader;

        virtual void _SetUpBuffers(const std::vector<float>& vertexData);
        virtual void _SetUpAttributes() = 0;
        virtual void _SetUpTexture(const char* fileName);
        virtual void _SetUpTransformations();
    
    private:
        unsigned int _texture;
        int _textureWidth, _textureHeight, _textureNumChannels;
        unsigned char* _textureImageData;
};

#endif