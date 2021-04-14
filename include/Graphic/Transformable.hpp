#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include <glm/glm.hpp>
#include <iostream>

class Transformable {
   public:
    Transformable();

    virtual ~Transformable() = 0;

    void transform(const glm::mat4 &transform);

    void translate(const glm::vec3 &position);

    void rotate(float angle, const glm::vec3 &axis);

    glm::mat4 getTransform() const;

   private:
    glm::mat4 m_transform;
};

#endif
