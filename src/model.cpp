#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>

Model::Model(string path) {
    LoadModel(path);
}

void Model::Draw(Shader& shader) {
    for(unsigned int = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}