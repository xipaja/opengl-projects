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
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, RECTANGLE_INDICES.size() * sizeof(int), RECTANGLE_INDICES.data(), GL_STATIC_DRAW);
}

void Rectangle::_SetUpAttributes() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Rectangle::Draw() {
    _customShader.Use();
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Rectangle::~Rectangle() {
    // Deallocate resources
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}
