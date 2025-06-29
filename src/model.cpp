#include "model.h"

Model::Model(std::string path) {
    _LoadModel(path);
}

void Model::_LoadModel(std::string path) {
     Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    // _ProcessNode(scene->mRootNode, scene);
}

void Model::Draw(Shader& shader) {
    for(unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}