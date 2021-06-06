#ifndef MODEL_HPP
#define MODEL_HPP

#include "Core/Export.hpp"
#include "Graphic/Drawable.hpp"
#include "Graphic/Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace te {

class TE_API Model : public Drawable {
   public:
    Model() = default;

    // This function check if the model is loaded before.
    // If loaded, take it out and skip the loadFromFile process.
    void loadModel(const std::string &path);

    void draw(RenderTarget &target, RenderStates states) const override;

    const std::vector<Mesh> &getMeshes() const;

    std::vector<Mesh> &getMeshes();
   private:
    template <typename, typename>
    friend class ResourceManager;

    // Costly model load function, will not check if loaded before.
    // Use loadModel for fast loading.
    bool load(const std::string &path);

    void processNode(aiNode *node, const aiScene *scene);

    void processMesh(aiMesh *mesh, const aiScene *scene);

    void processTextures(TextureArray &textures, aiMaterial *mat,
                         aiTextureType type);

    std::vector<Mesh> m_meshes;

    std::string m_directory;
};

}  // namespace te

#endif
