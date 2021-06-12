#ifndef MODEL_HPP
#define MODEL_HPP

#include "Core/Export.hpp"
#include "Graphic/Drawable.hpp"
#include "Graphic/Mesh.hpp"
#include "Graphic/VertexArray.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace te {

class TE_API Model : public Drawable {
   public:
    Model() = default;

    bool loadFromFile(const std::string &path);

    void draw(const Ref<Shader> &shader,
              const glm::mat4 &transform) const override;

    const std::vector<Mesh> &getMeshes() const;

    std::vector<Mesh> &getMeshes();

   private:
    void processNode(aiNode *node, const aiScene *scene);

    void processMesh(aiMesh *mesh, const aiScene *scene);

    void processTextures(Material &textures, aiMaterial *mat,
                         aiTextureType type);

    std::vector<Mesh> m_meshes;

    std::string m_directory;
};

}  // namespace te

#endif
