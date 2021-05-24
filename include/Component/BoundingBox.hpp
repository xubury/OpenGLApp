#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include <memory>
#include <Graphic/ElementBuffer.hpp>
#include <Graphic/Drawable.hpp>
#include <Entity/EntityBase.hpp>

class BoundingBox : public Component<BoundingBox, EntityBase> {
   public:
    BoundingBox();

    void initialize(const Vertex *vertex, int cnt);

    void update(const glm::mat4 &transform);

    const glm::vec3 &getLocalMin() const;

    const glm::vec3 &getLocalMax() const;

    const glm::vec3 &getWorldMax() const;

    const glm::vec3 &getWorldMin() const;

   private:
    glm::vec3 m_min;
    glm::vec3 m_max;
    glm::vec3 m_worldMin;
    glm::vec3 m_worldMax;
};

class BoundingBoxSystem : public System<BoundingBox, EntityBase> {
   public:
    BoundingBoxSystem();

    virtual void update(EntityManager<EntityBase> &manager,
                        const Time &deltaTime) override;

    void draw(EntityManager<EntityBase> &manager, RenderTarget &target,
              RenderStates states);

};

#endif
