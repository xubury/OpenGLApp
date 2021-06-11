#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>

namespace te {

class Transformable {
   public:
    Transformable();

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

    glm::vec3 getRight() const;

    glm::vec3 getUp() const;

    glm::vec3 getFront() const;

   protected:
    glm::mat4 m_transform;
};

}  // namespace te

#endif
