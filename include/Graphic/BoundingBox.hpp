#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include <memory>
#include <Graphic/ElementBuffer.hpp>
#include <Graphic/Drawable.hpp>

class BoundingBox : public Drawable {
   public:
    BoundingBox();

    virtual void draw(
        RenderTarget &target,
        RenderStates states = RenderStates::Default) const override;

    void initialize(const Vertex *vertex, int cnt);

    void update(float minX, float minY, float minZ, float maxX, float maxY,
                float maxZ);

    void update(const glm::mat4 &transform);

    glm::vec3 getLocalBounding() const;

   private:
    std::shared_ptr<ElementBuffer> m_elements;
    DebugVertex m_vertices[8];
    glm::vec3 m_min;
    glm::vec3 m_max;
};

#endif
