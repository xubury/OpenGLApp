#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <Graphic/Axis.hpp>
#include <ECS/ECS.hpp>

class Transform : public Component<Transform, DefaultEntity> {
   public:
    Transform();

    void draw(RenderTarget &target) const;

    void transform(const glm::mat4 &transform);

    void translate(const glm::vec3 &position);

    void rotate(float angle, const glm::vec3 &axis);

    glm::mat4 getTransform() const;

    glm::vec3 getPosition() const;

   private:
    glm::mat4 m_transform;
    Axis m_axis;
};

class TransformSystem : public System<Transform, DefaultEntity> {
   public:
    TransformSystem() = default;

    virtual void update(EntityManager<DefaultEntity> &manager,
                        const Time &deltaTime) override;

    void draw(EntityManager<DefaultEntity> &manager,
              RenderTarget &target) const;
};

#endif
