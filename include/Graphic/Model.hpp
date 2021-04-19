#ifndef MODEL_HPP
#define MODEL_HPP

#include <Graphic/Drawable.hpp>
#include <Graphic/Mesh.hpp>
#include <Graphic/BoundingBox.hpp>
#include <Graphic/Transformable.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ResourceManager.hpp>

class Model : public Drawable, public Transformable {
   public:
    Model() = default;

    // This function check if the model is loaded before.
    // If loaded, take it out and skip the loadFromFile process.
    void loadModel(const std::string &path);

    void draw(RenderTarget &target, RenderStates states) const override;

   private:
    template <typename, typename>
    friend class ResourceManager;

    // Costly model load function, will not check if loaded before.
    // Use loadModel for fast loading.
    bool loadFromFile(const std::string &path);

    void processNode(aiNode *node, const aiScene *scene);

    void processMesh(aiMesh *mesh, const aiScene *scene);

    void processTextures(TextureArray &textures, aiMaterial *mat,
                         aiTextureType type);

    std::vector<Mesh> m_meshes;

    std::string m_directory;

    static ResourceManager<std::string, Model> loadedModels;
};

#endif
