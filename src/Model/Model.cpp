#include "Model/Model.hpp"
#include "Core/Log.hpp"
#include "Utils/AssimpHelper.hpp"
#include <iostream>

namespace te {

void Model::draw(const Shader &shader, const glm::mat4 &transform) const {
    for (const auto &mesh : m_meshes) {
        mesh.draw(shader, transform);
    }
}

const std::vector<Mesh> &Model::getMeshes() const { return m_meshes; }

std::vector<Mesh> &Model::getMeshes() { return m_meshes; }

bool Model::loadFromFile(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::string error = import.GetErrorString();
        TE_CORE_ERROR("Assimp Error:{}", error);
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
    Ref<Material> textures = createRef<Material>();
    for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.position = AssimpHelper::getGLMVec(mesh->mVertices[i]);
        vertex.normal = AssimpHelper::getGLMVec(mesh->mNormals[i]);

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

    processBoneWeight(vertices, mesh);

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    processTextures(*textures, material, aiTextureType_AMBIENT);
    processTextures(*textures, material, aiTextureType_DIFFUSE);
    processTextures(*textures, material, aiTextureType_SPECULAR);
    m_meshes.emplace_back((GLenum)mesh->mPrimitiveTypes, vertices, indices,
                          textures);
}

void Model::processBoneWeight(std::vector<Vertex> &vertices, aiMesh *mesh) {
    for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
        int boneId = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (m_boneInfoMap.count(boneName) == 0) {
            BoneInfo boneInfo;
            boneInfo.id = m_boneCounter;
            boneInfo.offset = AssimpHelper::covertMatrixToGLM(
                mesh->mBones[boneIndex]->mOffsetMatrix);
            m_boneInfoMap[boneName] = boneInfo;
            boneId = m_boneCounter;
            ++m_boneCounter;
        } else {
            boneId = m_boneInfoMap[boneName].id;
        }
        TE_CORE_ASSERT(boneId != -1, "bone info error");
        aiVertexWeight *weights = mesh->mBones[boneIndex]->mWeights;
        uint32_t numWeight = mesh->mBones[boneIndex]->mNumWeights;
        for (uint32_t weightIndex = 0; weightIndex < numWeight; ++weightIndex) {
            uint32_t vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            TE_CORE_ASSERT(vertexId < vertices.size(), "invalid vertex index");
            vertices[vertexId].setVertexBoneData(boneId, weight);
        }
    }
}

void Model::processTextures(Material &textures, aiMaterial *mat,
                            aiTextureType type) {
    for (std::size_t i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString relativePath;
        mat->GetTexture(type, i, &relativePath);
        std::string path = m_directory + relativePath.C_Str();
        switch (type) {
            case aiTextureType_AMBIENT:
                textures.emplace(m_loadedTexture.getOrLoad(path, path),
                                 Material::TEXTURE_AMBIENT);
                break;
            case aiTextureType_DIFFUSE:
                textures.emplace(m_loadedTexture.getOrLoad(path, path),
                                 Material::TEXTURE_DIFFUSE);
                break;
            case aiTextureType_SPECULAR:
                textures.emplace(m_loadedTexture.getOrLoad(path, path),
                                 Material::TEXTURE_SPECULAR);
                break;
            default:
                break;
        }
    }
}

}  // namespace te
