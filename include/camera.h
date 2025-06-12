#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 0.005f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;
const glm::vec3 DEFAULT_POS     = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 DEFAULT_UP      = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_FRONT   = glm::vec3(0.0f, 0.0f, -1.0f);

class Camera {
    public:
        // Camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw;
        float Pitch;

        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        Camera();
        glm::mat4 GetViewMatrix();
        void ProcessKeyboardInput(float deltaTime);
        void ProcessMouseMovement(float xOffset, float yOffset);
        void ProcessMouseScroll(float scrollValue);

    private:
        void _UpdateCameraVectors();
};

#endif