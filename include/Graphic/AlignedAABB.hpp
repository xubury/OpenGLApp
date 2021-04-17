#ifndef ALIGNED_ABB_HPP
#define ALIGNED_ABB_HPP

#include <Graphic/ElementBuffer.hpp>
#include <Graphic/Drawable.hpp>

class AlignedAABB : public Drawable {
   public:
    AlignedAABB();

    virtual void draw(RenderTarget &target, RenderStates states) const override;

    void calculateAABB(Vertex *vertex, int cnt);

    void setBoundingBox(float minX, float minY, float minZ, float maxX,
                        float maxY, float maxZ);

    void updateAABB(const glm::mat4 &transform);

   private:
    ElementBuffer m_elements;
    glm::vec3 m_originalPosition[8];
    glm::vec3 m_translate;
    Vertex m_updatedVertices[8];
};

#endif
