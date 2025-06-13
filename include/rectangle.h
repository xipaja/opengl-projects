#ifndef RECT_H
#define RECT_H

#include "geometry.h"

class Rectangle : public Geometry {
    public:
        Rectangle(const char* vertPath, const char* fragPath);
        ~Rectangle();
        void Draw() override;

    private:
        unsigned int _EBO;
        void _SetUpBuffers(const std::vector<float>& vertexData) override;
        void _SetUpAttributes() override;
};

#endif