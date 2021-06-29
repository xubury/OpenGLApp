#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace te {

class Transformable {
   public:
    Transformable();

    void setTransform(const glm::mat4 &transform);

    void setRotation(const glm::quat &rotation);

    glm::quat getRotation() const;

    void translateLocal(const glm::vec3 &t);

    void translateWorld(const glm::vec3 &t);

    void rotateLocal(float radians, const glm::vec3 &axis);

    void rotateWorld(float radians, const glm::vec3 &axis);

    void rotateLocal(const glm::quat &rotation);

    void rotateWorld(const glm::quat &rotation);

    void setEulerAngle(glm::vec3 eulerAngle);

    glm::vec3 getEulerAngle() const;

    glm::mat4 getTransform() const;

    void setPosition(const glm::vec3 &position);

    glm::vec3 getPosition() const;

    glm::vec3 getLeft() const;

    glm::vec3 getUp() const;

    glm::vec3 getFront() const;

    glm::vec3 toLocalSpace(const glm::vec3 &world) const;

    glm::vec3 toWorldSpace(const glm::vec3 &local) const;
   protected:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
};

}  // namespace te

#endif
