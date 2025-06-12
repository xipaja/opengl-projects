#include "cube.h"
#include "vertices.h"

Cube::Cube(const char* vertPath, const char* fragPath) : Geometry(vertPath, fragPath) {
    _SetUpBuffers(CUBE_EXAMPLE_VERTICES);
    _SetUpAttributes();
    _SetUpTexture("assets/fun_pattern.jpg");
}

void Cube::_SetUpAttributes() {
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Cube::SetUpCamViewTransform(glm::mat4 viewMatrix) {
    _customShader.SetMat4("view", viewMatrix);
}

void Cube::SetUpTransformations() {
    // ------------- Rotating cubes ------------- 
    // for (unsigned int i = 0; i < 10; i++) {
    //     float angle = 20.0f * i;
    //     glm::mat4 model = glm::mat4(1.0f);
        
    //     model = glm::translate(model, CUBES_EXAMPLE_POSITIONS[i]);
    //     model = glm::rotate(model, (SDL_GetTicks() / 1000.0f) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    //     _customShader.SetMat4("model", model);
    //     glDrawArrays(GL_TRIANGLES, 0, 36);
    // }

    // ------------- Static cube in center ------------- 
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, CUBES_EXAMPLE_POSITIONS[0]);
    _customShader.SetMat4("model", modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::Draw() {
    BindTexture();
    _customShader.Use();
    BindVertexArray();
    SetUpTransformations();
}