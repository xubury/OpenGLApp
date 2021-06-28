#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "Graphic/VertexArray.hpp"
#include "Graphic/Material.hpp"
#include "ECS/ECS.hpp"

namespace te {

class Terrain : public EntityBase {
   public:
    Terrain(EntityManager<EntityBase> *manager, uint32_t id, int gridSize,
            int vertexCount, Ref<Material> material);

    void draw(const Shader &shader, const glm::mat4 &transform) const override;

    float height(float localX, float localZ) const;

   private:
    void computeNormal();

    Ref<VertexArray> m_terrain;
    Ref<Material> m_material;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    int m_gridSize;
    int m_vertexCount;
};

}  // namespace te

#endif /* TERRAIN_HPP */
