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

    _directory = path.substr(0, path.find_last_of('/'));

    _ProcessNode(scene->mRootNode, scene);
}

void Model::_ProcessNode(aiNode* node, const aiScene* scene) {
    // Process all node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(_ProcessMesh(mesh, scene));
    }

    // Same for node's children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        _ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::_ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Process vertex positions, normals, tex coords
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Get vert pos from mesh vertex position array from assimp
        glm::vec3 pos;
        pos.x = mesh->mVertices[i].x;
        pos.y = mesh->mVertices[i].y;
        pos.z = mesh->mVertices[i].z;
        vertex.Position = pos;

        // Same, for normals
        if (mesh->HasNormals()) {
            pos.x = mesh->mNormals[i].x;
            pos.y = mesh->mNormals[i].y;
            pos.z = mesh->mNormals[i].z;
            vertex.Normal = pos;
        }

        // Now texture coords
        if (mesh->mTextureCoords[0]) {
            glm::vec2 tex;
            tex.x = mesh->mTextureCoords[0][i].x;
            tex.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = glm::vec3(tex, 0.0f);
        } else {
            vertex.TexCoords = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices for element buffer
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
 
    // Process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<Texture> diffuseMaps = _LoadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = _LoadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::_LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString path;
        mat->GetTexture(type, i, &path);
        // Check if we've already loaded this texture
        bool skipLoad = false;
        for (unsigned int j = 0; j < _textures_loaded.size(); j++) {
            if (std::strcmp(_textures_loaded[j].path.data(), path.C_Str()) == 0) {
                textures.push_back(_textures_loaded[j]);
                skipLoad = true;
                break;
            }
        }

        if (!skipLoad) {
            Texture tex;
            tex.id = _TextureFromFile(path.C_Str(), _directory);
            tex.type = typeName;
            tex.path = path.C_Str();
            textures.push_back(tex);
            _textures_loaded.push_back(tex);
        }
        
    }
    
    return textures;
}

void Model::Draw(Shader& shader) {
    for(unsigned int i = 0; i < _meshes.size(); i++) {
        _meshes[i].Draw(shader);
    }
}

unsigned int Model::_TextureFromFile(const char *path, const std::string &dir, bool gamma)
{
    std::string filename = std::string(path);
    filename = dir + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}