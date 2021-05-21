#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <Graphic/Axis.hpp>
#include <ECS/ECS.hpp>

class Transform : public Component<Transform, EntityBase> {
   public:
    Transform();

    void transform(const glm::mat4 &transform);

    void translateLocal(const glm::vec3 &t);

    void translateWorld(const glm::vec3 &t);

    void rotateLocal(float radians, const glm::vec3 &axis);

    void rotateWorld(float radians, const glm::vec3 &axis);

    // Factor as RyRxRz,
    // details in https://www.geometrictools.com/Documentation/EulerAngles.pdf
    void setEulerAngle(glm::vec3 eulerAngle);

    // Factor as RyRxRz,
    // details in https://www.geometrictools.com/Documentation/EulerAngles.pdf
    glm::vec3 getEulerAngle() const;

    const glm::mat4 &getTransform() const;

    void setPosition(const glm::vec3 &position);

    glm::vec3 getPosition() const;


   protected:
    glm::mat4 m_transform;
};

class TransformSystem : public System<Transform, EntityBase> {
   public:
    TransformSystem();

    virtual void update(EntityManager<EntityBase> &manager,
                        const Time &deltaTime) override;

    void draw(EntityManager<EntityBase> &manager, RenderTarget &target,
              RenderStates states) const;

   private:
    static Axis s_axis;
};

#endif
