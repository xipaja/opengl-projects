#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include "shader.h"

int g_screenHeight = 500;
int g_screenWidth = 500;
SDL_Window* g_GraphicsWindow = nullptr;
SDL_GLContext g_OpenGLContext = nullptr;

bool g_Quit = false;

void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL3 could not initialize video subsystem" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    g_GraphicsWindow = SDL_CreateWindow("My OpenGL Window",
                                    g_screenWidth,
                                    g_screenHeight,
                                    SDL_WINDOW_OPENGL);
    if(g_GraphicsWindow == nullptr) {
        std::cout << "SDL window was unable to be created" << std::endl;
        exit(1);
    }

    g_OpenGLContext = SDL_GL_CreateContext(g_GraphicsWindow);
    if(g_OpenGLContext == nullptr) {
        std::cout << "OpenGL Context not available." << std::endl;
        exit(1);
    }
    
    // Init the Glad Library
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Could not init glad library" << std::endl;
        exit(1);
    }

    glViewport(0, 0, g_screenHeight, g_screenWidth);
}

// TODO: figure out resizing with SDL
// void framebuffer_size_callback(SDL_Window* window, int width, int height) {
//     glViewport(0, 0, width, height);
// }

void processInputs(SDL_Window* window) {
    // process input with SDl
}

void pollEvents() {
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_EVENT_QUIT) {
            std::cout << "Goodbye!" << std::endl;
            g_Quit = true;
        }
    }
}

int main() {
    init();

    Shader customShader("shaders/vertex.vs", "shaders/fragment.fs");

    // Set up vertex data and buffers, configure vertex attributes
    float vertices[] = {
        // positions        // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    // Store vertex data, managed by VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind VAO first, then bind and set VBOs, then configure attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex data per vertex attribute
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Render loop
    while(!g_Quit) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        customShader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(g_GraphicsWindow);
        pollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

