#ifndef MODEL_HPP
#define MODEL_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Export.hpp"
#include "Core/ResourceManager.hpp"
#include "Graphic/Drawable.hpp"
#include "Graphic/VertexArray.hpp"

#include "Model/Mesh.hpp"
#include "Model/Bone.hpp"

namespace te {

class TE_API Model : public Drawable {
   public:
    Model() = default;

    bool loadFromFile(const std::string &path);

    void draw(const Shader &shader, const glm::mat4 &transform) const override;

    const std::vector<Mesh> &getMeshes() const;

    std::vector<Mesh> &getMeshes();

    const std::unordered_map<std::string, BoneInfo> &getBoneInfoMap() const {
        return m_boneInfoMap;
    }

    std::unordered_map<std::string, BoneInfo> &getBoneInfoMap() {
        return m_boneInfoMap;
    }

    uint32_t getBoneCounter() const { return m_boneCounter; }

    void setBoneCounter(uint32_t counter) { m_boneCounter = counter; }

   private:
    void processNode(aiNode *node, const aiScene *scene);

    void processMesh(aiMesh *mesh, const aiScene *scene);

    void processBoneWeight(std::vector<Vertex> &vertices, aiMesh *mesh);

    void processTextures(Material &textures, aiMaterial *mat,
                         aiTextureType type);

    std::vector<Mesh> m_meshes;

    std::string m_directory;

    ResourceManager<std::string, Texture> m_loadedTexture;
    std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
    uint32_t m_boneCounter;
};

}  // namespace te

#endif
