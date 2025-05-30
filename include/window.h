#ifndef WINDOW_H
#define WINDOW_H

// #include <SDL3/SDL.h>
// #include <glad/glad.h>
#include <iostream>
// #include <math.h>
// #include "shader.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <camera.h>

class Window {
    public:
        Window();

    private:
        int m_windowWidth;
        int m_windowHeight;
};

Window::Window() : m_windowWidth(600), m_windowHeight(500) {
    std::cout << "Created Window! W: " << m_windowWidth << " H: " << m_windowHeight << std::endl;
};

#endif