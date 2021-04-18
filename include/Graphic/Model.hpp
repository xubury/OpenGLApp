#ifndef MODEL_HPP
#define MODEL_HPP

#include <Graphic/Drawable.hpp>
#include <Graphic/Mesh.hpp>
#include <Graphic/BoundingBox.hpp>
#include <Graphic/Transformable.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Drawable, public Transformable {
   public:
    Model();

    void loadModel(const std::string &path);

    void draw(RenderTarget &target, RenderStates states) const override;

   private:
    void processNode(aiNode *node, const aiScene *scene);

    void processMesh(aiMesh *mesh, const aiScene *scene);

    void processTextures(TextureArray &textures, aiMaterial *mat,
                         aiTextureType type);

    std::vector<Mesh> m_meshes;

    std::string m_directory;
};

#endif
