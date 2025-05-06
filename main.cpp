#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>

// Globals
int gScreenHeight = 640;
int gScreenWidth = 480;
SDL_Window* gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

bool gQuit = false;

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void InitializeProgram() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL3 could not initialize video subsystem" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gGraphicsAppWindow = SDL_CreateWindow("OpenGL Window", 
                        gScreenWidth, gScreenHeight, 
                        SDL_WINDOW_OPENGL);
    if (gGraphicsAppWindow == nullptr) {
        std::cout << "SDL_window was not able to be created." << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsAppWindow);
    if(gOpenGLContext == nullptr) {
        std::cout << "OpenGL Context not available." << std::endl;
        exit(1);
    }

    // Init the Glad Library
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Could not init glad library" << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
}

void Input() {
    SDL_Event e;

    // Handle events
    while(SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_EVENT_QUIT) {
            std::cout << "Goodbye!" << std::endl;
            gQuit = true;
        }
    }
}

void PreDraw() {

}

void Draw() {

}

void MainLoop() {
    while(!gQuit) {
        Input();

        PreDraw();
        
        Draw();

        // Update the screen
        SDL_GL_SwapWindow(gGraphicsAppWindow);
    }
}

void CleanUp() {
    SDL_DestroyWindow(gGraphicsAppWindow);
    SDL_Quit();
}

int main() {

    InitializeProgram();

    MainLoop();

    CleanUp();
    
    return 0;
}