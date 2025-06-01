#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <SDL3/SDL.h>
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

// Constructor
Camera::Camera() :
Position(DEFAULT_POS),
Up(DEFAULT_UP),
Front(DEFAULT_FRONT),
WorldUp(DEFAULT_UP),
Yaw(YAW),
Pitch(PITCH),
MovementSpeed(SPEED), 
MouseSensitivity(SENSITIVITY), 
Zoom(ZOOM) {
    _UpdateCameraVectors();
}

// Get view matrix calculated using yaw/pitch Euler angles and lookAt matrix
glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
};

void Camera::ProcessKeyboardInput(float deltaTime) {
    const bool* state = SDL_GetKeyboardState(nullptr);
    const float cameraVelocity = MovementSpeed * deltaTime;

    if (state[SDL_SCANCODE_W]) {
        // Move forward
        Position += cameraVelocity * Front;
    }
    else if (state[SDL_SCANCODE_A]) {
        // Move left
        Position -= cameraVelocity * Right;
    }
    else if (state[SDL_SCANCODE_S]) {
        // Move back
        Position -= cameraVelocity * Front;
    }
    else if (state[SDL_SCANCODE_D]) {
        // Move right
        Position += cameraVelocity * Right;
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

        // Avoid weird pitch camera angles
    if (Pitch > 89.0f) {
        Pitch = 89.0f;
    }
    if (Pitch < -89.0f) {
        Pitch = -89.0f;
    }

    _UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float scrollValue) {
    if (scrollValue > 0) {
        Zoom -= 5.0f;
        if (Zoom < 1.0f) {
            Zoom = 1.0f;
        }
    } else {
        Zoom += 5.0f;
        if (Zoom > 90.0f) {
            Zoom = 90.0f;
        }
    }
}

void Camera::_UpdateCameraVectors() {
    // Calc new front vector from updated angles
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(Yaw) * cos(glm::radians(Pitch)));
    newFront.y = sin(glm::radians(Pitch));
    newFront.z = sin(glm::radians(Yaw) * cos(glm::radians(Pitch)));
    Front = glm::normalize(newFront);

    // Calc new right and up vectors
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

#endif