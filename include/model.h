#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <vector>
#include <string>

class Model {
    public:
        Model(std::string path);
        void Draw(Shader& shader);
    
    private:
        std::vector<Mesh> _meshes;
        std::string _directory;
        std::vector<Texture> _textures_loaded;
        
        void _LoadModel(std::string path);
        void _ProcessNode(aiNode* node, const aiScene* scene);
        Mesh _ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> _LoadMaterialTextures(aiMaterial* mat, aiTextureType texType, std::string typeName);
        unsigned int _TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};

#endif