#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
// #include <math.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

class Window {
    public:
        Window();
        void initWindow();
        void pollEvents();
        void processMouse(float xPos, float yPos);
        void draw();

        void setCustomShader(const char* vertexFilePath, const char* fragmentFilePath);

    private:
        int m_windowWidth;
        int m_windowHeight;
        SDL_Window* m_window;
        SDL_GLContext m_OpenGLContext;
        bool m_Quit;

        // Camera variables
        Camera m_camera;
        float m_lastX;
        float m_lastY;
        bool m_firstMouse;
        float m_deltaTime;
        float m_lastFrameTime;

        // Shader variables
        Shader m_customShader;
};

Window::Window() : 
m_windowWidth(600), 
m_windowHeight(500), 
m_window(nullptr), 
m_OpenGLContext(nullptr),
m_lastX(m_windowWidth / 2.0f),
m_lastY(m_windowHeight / 2.0f),
m_firstMouse(true),
m_deltaTime(0.0f),
m_lastFrameTime(0.0f),
m_Quit(false),
m_customShader("", "")
{
    std::cout << "Created Window! W: " << m_windowWidth << " H: " << m_windowHeight << std::endl;
};

void Window::initWindow() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL3 could not initialize video subsystem" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_window = SDL_CreateWindow("WINDOW::My OpenGL Window",
                                    m_windowWidth,
                                    m_windowHeight,
                                    SDL_WINDOW_OPENGL);
    if (m_window == nullptr) {
        std::cout << "SDL window was unable to be created" << std::endl;
        exit(1);
    }

    m_OpenGLContext = SDL_GL_CreateContext(m_window);
    if (m_OpenGLContext == nullptr) {
        std::cout << "OpenGL Context not available." << std::endl;
        exit(1);
    }
    
    // Init the Glad Library
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Could not init glad library" << std::endl;
        exit(1);
    }

    glViewport(0, 0, m_windowWidth, m_windowHeight);
}

void Window::pollEvents() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_EVENT_QUIT) {
            std::cout << "Goodbye from Window!" << std::endl;
            m_Quit = true;
        }
        if (e.type == SDL_EVENT_MOUSE_MOTION) {
            processMouse(e.motion.x, e.motion.y);
        }
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            m_camera.processMouseScroll(e.motion.x);
        }
    }
}

void Window::processMouse(float xPos, float yPos) {
    // Avoid window capturing the entering mouse pos as the initial camera pos
    if (m_firstMouse) {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstMouse = false;
    }

    float xOffset = xPos - m_lastX;
    // Y is reversed for bottom to top Y coordinates
    float yOffset = m_lastY - yPos;

    m_camera.processMouseMovement(xOffset, yOffset);
}

void Window::draw() {
    initWindow();
}


#endif