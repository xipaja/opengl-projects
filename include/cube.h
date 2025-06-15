#ifndef CUBE_H
#define CUBE_H

#include "geometry.h"

class Cube : public Geometry {
    public:
        Cube(const char* vertPath, const char* fragPath);
        void SetUpCamViewTransform(glm::mat4 viewMatrix) override;
        void _SetUpTransformations() override;
        void Draw() override;
    
    private:
        void _SetUpAttributes() override;
};

#endif