#import <vector>
#import <string>
#import "shader.h"
#import "mesh.h"

class Model {
    public:
        Model(string path);
        void Draw(Shader& shader);
    
    private:
        std::vector<Mesh> meshes;
        string directory;

        void LoadMdoel(string path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType texType, string typeName);
};