#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include <Graphic/ElementBuffer.hpp>
#include <Graphic/Drawable.hpp>

class BoundingBox : public Drawable {
   public:
    BoundingBox();

    virtual void draw(RenderTarget &target, RenderStates states) const override;

    void initialize(Vertex *vertex, int cnt);

    void update(float minX, float minY, float minZ, float maxX, float maxY,
                float maxZ);

    void update(const glm::mat4 &transform);

   private:
    ElementBuffer m_elements;
    glm::vec3 m_originalPosition[8];
    Vertex m_updatedVertices[8];
};

#endif
