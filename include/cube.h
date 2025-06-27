#ifndef CUBE_H
#define CUBE_H

#include "geometry.h"

class Cube : public Geometry {
    public:
        Cube(const char* vertPath, const char* fragPath);
        void SetUpCamViewTransform(glm::mat4 viewMatrix) override;
        void SetModelMatrix(glm::mat4 modelMatrix) override;
        void _SetUpTransformations(glm::mat4 modelMatrix);
        void Draw() override;
    
    private:
        glm::mat4 _modelMatrix;
        void _SetUpAttributes() override;
};

#endif