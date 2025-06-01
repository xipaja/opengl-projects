#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "geometry.h"

const float DEFAULT_WIDTH = 500;
const float DEFAULT_HEIGHT = 500;
const float DEFAULT_LAST_X = DEFAULT_WIDTH / 2.0f;
const float DEFAULT_LAST_Y = DEFAULT_HEIGHT / 2.0f;

class Window {
    public:
        Window();
        void InitWindow();
        void PollEvents();
        void ProcessMouse(float xPos, float yPos);
        void Draw();

    private:
        int _windowWidth;
        int _windowHeight;
        SDL_Window* _window;
        SDL_GLContext _OpenGLContext;
        bool _Quit;

        // Camera variables
        Camera _camera;
        float _lastX;
        float _lastY;
        bool _firstMouse;
        float _deltaTime;
        float _lastFrameTime;
};

Window::Window() : 
_windowWidth(DEFAULT_WIDTH), 
_windowHeight(DEFAULT_HEIGHT), 
_window(nullptr), 
_OpenGLContext(nullptr),
_lastX(DEFAULT_LAST_X),
_lastY(DEFAULT_LAST_Y),
_firstMouse(true),
_deltaTime(0.0f),
_lastFrameTime(0.0f),
_Quit(false)
{
    std::cout << "Created Window! W: " << _windowWidth << " H: " << _windowHeight << std::endl;
};

void Window::InitWindow() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL3 could not initialize video subsystem" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    _window = SDL_CreateWindow("WINDOW::My OpenGL Window",
                                    _windowWidth,
                                    _windowHeight,
                                    SDL_WINDOW_OPENGL);
    if (_window == nullptr) {
        std::cout << "SDL window was unable to be created" << std::endl;
        exit(1);
    }

    _OpenGLContext = SDL_GL_CreateContext(_window);
    if (_OpenGLContext == nullptr) {
        std::cout << "OpenGL Context not available." << std::endl;
        exit(1);
    }
    
    // Init the Glad Library
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Could not init glad library" << std::endl;
        exit(1);
    }

    glViewport(0, 0, _windowWidth, _windowHeight);
}

void Window::PollEvents() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_EVENT_QUIT) {
            std::cout << "Goodbye from Window!" << std::endl;
            _Quit = true;
        }
        if (e.type == SDL_EVENT_MOUSE_MOTION) {
            ProcessMouse(e.motion.x, e.motion.y);
        }
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            _camera.processMouseScroll(e.motion.x);
        }
    }
}

void Window::ProcessMouse(float xPos, float yPos) {
    // Avoid window capturing the entering mouse pos as the initial camera pos
    if (_firstMouse) {
        _lastX = xPos;
        _lastY = yPos;
        _firstMouse = false;
    }

    float xOffset = xPos - _lastX;
    // Y is reversed for bottom to top Y coordinates
    float yOffset = _lastY - yPos;

    _lastX = xPos;
    _lastY = yPos;
    
    _camera.processMouseMovement(xOffset, yOffset);
}

void Window::Draw() {
    InitWindow();
    glEnable(GL_DEPTH_TEST);

    Geometry geometry;

    while (!_Quit) {
        float currentFrameTime = SDL_GetTicks();
        _deltaTime = currentFrameTime - _lastFrameTime;
        _lastFrameTime = currentFrameTime;

        _camera.processKeyboardInput(_deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        geometry.BindTexture();
        geometry.GetShader().use();

        // Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(_camera.Zoom), (float)_windowWidth / (float)_windowHeight, 0.1f, 100.0f);
        int projLocation = glGetUniformLocation(geometry.GetShader().id, "projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

        // Cam/view transformation
        glm::mat4 view = _camera.getViewMatrix();
        int viewLocation = glGetUniformLocation(geometry.GetShader().id, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        
        geometry.BindVertexArray();

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, CUBE_POSITIONS[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (SDL_GetTicks() / 1000.0f) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            int modelLocation = glGetUniformLocation(geometry.GetShader().id, "model");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        SDL_GL_SwapWindow(_window);
        PollEvents();
    }
}

#endif