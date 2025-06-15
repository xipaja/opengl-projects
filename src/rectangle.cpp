#include "rectangle.h"
#include "vertices.h"

Rectangle::Rectangle(const char* vertPath, const char* fragPath) : Geometry(vertPath, fragPath) {
    _SetUpBuffers(RECTANGLE_VERTICES);
    _SetUpAttributes();
}

void Rectangle::_SetUpBuffers(const std::vector<float>& vertexData) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);
    // Bind VAO, then bind and set VBO, then config attributes
    glBindVertexArray(_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, RECTANGLE_INDICES.size() * sizeof(int), RECTANGLE_INDICES.data(), GL_STATIC_DRAW);
}

void Rectangle::_SetUpAttributes() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
}

void Rectangle::_SetUpTransformations() {
    glm::mat4 transform = glm::mat4(1.0f);
    // transformation = glm::translate(transformation, glm::vec3(0.0f, -0.5f, 0.0f));
    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    transform = glm::scale(transform, glm::vec3(1.5, 0.5, 0.5));

    _customShader.Use();
    _customShader.SetMat4("transform", transform);
}

void Rectangle::SetUpCamViewTransform(glm::mat4 viewMatrix) {
    _customShader.SetMat4("view", viewMatrix);
}

void Rectangle::Draw() {
    _SetUpTransformations();
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

Rectangle::~Rectangle() {
    // Deallocate resources
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}
