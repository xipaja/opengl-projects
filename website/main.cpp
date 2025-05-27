#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.h>

int g_screenHeight = 500;
int g_screenWidth = 500;
SDL_Window* g_GraphicsWindow = nullptr;
SDL_GLContext g_OpenGLContext = nullptr;

// Camera variables
Camera camera;
float lastX = g_screenWidth / 2.0f;
float lastY = g_screenHeight / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // Time between curr frame and last frame
float lastFrameTime = 0.0f; // Time of last frame

bool g_Quit = false;

void initWindow() {
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
    if (g_GraphicsWindow == nullptr) {
        std::cout << "SDL window was unable to be created" << std::endl;
        exit(1);
    }

    g_OpenGLContext = SDL_GL_CreateContext(g_GraphicsWindow);
    if (g_OpenGLContext == nullptr) {
        std::cout << "OpenGL Context not available." << std::endl;
        exit(1);
    }
    
    // Init the Glad Library
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Could not init glad library" << std::endl;
        exit(1);
    }

    glViewport(0, 0, g_screenHeight, g_screenWidth);
}

void processMouse(float xPos, float yPos) {
    // Avoid window capturing the entering mouse pos as the initial camera pos
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // reversed for bottom -> top y coordinates
    lastX = xPos;
    lastY = yPos;

    camera.processMouseMovement(xOffset, yOffset);
}

void pollEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_EVENT_QUIT) {
            std::cout << "Goodbye!" << std::endl;
            g_Quit = true;
        }
        if (e.type == SDL_EVENT_MOUSE_MOTION) {
            processMouse(e.motion.x, e.motion.y);
        }
        if (e.type == SDL_EVENT_MOUSE_WHEEL) {
            camera.processMouseScroll(e.motion.x);
        }
    }
}

int main() {
    initWindow();

    glEnable(GL_DEPTH_TEST);
    
    Shader customShader("shaders/vertex.vs", "shaders/fragment.fs");

    // 3D cube!
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Texture - set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // For minified textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // For magnified textures

    // Texture - load
    int width, height, numChannels;
    unsigned char* texImageData = stbi_load("container.jpg", &width, &height, &numChannels, 0);

    // Texture - generate
    if (texImageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texImageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "ERROR::TEXTURE::Failed to load texture" << std::endl;
    }

    // Texture - Free image memory
    stbi_image_free(texImageData);

    // Render loop
    while (!g_Quit) {
        float currentFrameTime = SDL_GetTicks();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        camera.processKeyboardInput(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        customShader.use();

        // Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)g_screenWidth / (float)g_screenHeight, 0.1f, 100.0f);
        int projLocation = glGetUniformLocation(customShader.id, "projection");
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

        // Cam/view transform
        glm::mat4 view = camera.getViewMatrix();
        int viewLocation = glGetUniformLocation(customShader.id, "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (SDL_GetTicks() / 1000.0f) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            int modelLocation = glGetUniformLocation(customShader.id, "model");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        SDL_GL_SwapWindow(g_GraphicsWindow);
        pollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

