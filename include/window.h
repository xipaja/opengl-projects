#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include "camera.h"

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

#endif