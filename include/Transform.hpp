#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>

class Transform {
   public:
    Transform();

    void transform(const glm::mat4 &transform);

    void translate(const glm::vec3 &position);

    void rotate(float angle, const glm::vec3 &axis);

    glm::mat4 getTransform() const;

   private:
    glm::mat4 m_transform;
};

#endif
