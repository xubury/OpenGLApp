#include <Component/Light.hpp>
#include <Component/Transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

glm::mat4 Light::getLightSpaceMatrix() const {
    auto trans = owner()->component<Transform>();
    const glm::vec3 &up = trans->getUp();
    const glm::vec3 &front = trans->getFront();
    const glm::vec3 &pos = trans->getPosition();
    glm::mat4 lightView = glm::lookAt(pos, pos + front, up);
    float near_plane = 0.1f, far_plane = 100.f;
    glm::mat4 lightProjection =
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    return lightProjection * lightView;
}

glm::vec3 Light::getDirection() const {
    return owner()->component<Transform>()->getFront();
}

glm::vec3 Light::getPosition() const {
    return owner()->component<Transform>()->getPosition();
}
