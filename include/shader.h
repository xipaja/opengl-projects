#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        unsigned int id;

        // Constructor reads and bulids shader
        Shader(const char* vertexFilePath, const char* fragmentFilePath);
        // Use/activate shader
        void Use();
        // Utility uniform functions
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetMat4(const std::string& name, glm::mat4 matrix) const;
        void SetVec3(const std::string& name, float x, float y, float z) const;
        void SetVec3(const std::string& name, glm::vec3 vec) const;
};

#endif