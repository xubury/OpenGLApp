#include <Graphic/Model.hpp>
#include <Graphic/RenderTarget.hpp>

ResourceManager<std::string, Model> Model::loadedModels;

void Model::loadModel(const std::string &path) {
    *this = loadedModels.getOrLoad(path, path);
    for (const auto &mesh : m_meshes) {
        m_aabb.initialize(mesh.getVertex(), mesh.size());
    }
}

void Model::draw(RenderTarget &target, RenderStates states) const {
    states.transform = getTransform();
    for (const auto &mesh : m_meshes) {
        mesh.draw(target, states);
    }
    m_aabb.draw(target);
}

bool Model::loadFromFile(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return false;
    }
    m_directory = path.substr(0, path.find_last_of('/') + 1);
    processNode(scene->mRootNode, scene);
    return true;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (std::size_t i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    TextureArray textures;
    for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;
        if (mesh->mTextureCoords[0]) {
            vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.texCoord = glm::vec2(0);
        }
        vertices.push_back(vertex);
    }

    for (std::size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (std::size_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    processTextures(textures, material, aiTextureType_DIFFUSE);
    processTextures(textures, material, aiTextureType_SPECULAR);
    m_meshes.emplace_back(vertices, indices, textures);
}

void Model::processTextures(TextureArray &textures, aiMaterial *mat,
                            aiTextureType type) {
    for (std::size_t i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString path;
        mat->GetTexture(type, i, &path);
        switch (type) {
            case aiTextureType_DIFFUSE:
                textures.loadFromFile(m_directory + path.C_Str(),
                                      Texture::DIFFUSE);
                break;
            case aiTextureType_SPECULAR:
                textures.loadFromFile(m_directory + path.C_Str(),
                                      Texture::SPECULAR);
                break;
            default:
                break;
        }
    }
}
