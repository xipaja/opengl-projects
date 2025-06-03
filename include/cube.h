#ifndef CUBE_H
#define CUBE_H

#include <geometry.h>

class Cube : public Geometry {
    public:
        Cube();
        void SetUpProjectionMatrix(float zoom, float aspectRatio) override;
        void SetUpCamViewTransform(glm::mat4 viewMatrix);
        void SetUpTransformations() override;
};

Cube::Cube() : Geometry() {}

void Cube::SetUpProjectionMatrix(float zoom, float aspectRatio) {
    float zNear = 0.1f;
    float zFar = 100.0f;
    glm::mat4 projection = glm::perspective(glm::radians(zoom), aspectRatio, zNear, zFar);
    int projLocation = glGetUniformLocation(_customShader.id, "projection");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void Cube::SetUpCamViewTransform(glm::mat4 viewMatrix) {
    int viewLocation = glGetUniformLocation(_customShader.id, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void Cube::SetUpTransformations() {
    for (unsigned int i = 0; i < 10; i++) {
        float angle = 20.0f * i;
        glm::mat4 model = glm::mat4(1.0f);
        
        model = glm::translate(model, CUBES_EXAMPLE_POSITIONS[i]);
        model = glm::rotate(model, (SDL_GetTicks() / 1000.0f) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

        int modelLocation = glGetUniformLocation(_customShader.id, "model");
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

#endif