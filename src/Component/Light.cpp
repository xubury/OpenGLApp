#include <Component/Light.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace te {

glm::mat4 Light::getLightSpaceMatrix() const {
    const glm::vec3 &up = owner()->getUp();
    const glm::vec3 &front = owner()->getFront();
    const glm::vec3 &pos = owner()->getPosition();
    glm::mat4 lightView = glm::lookAt(pos, pos + front, up);
    float near_plane = 0.1f, far_plane = 100.f;
    glm::mat4 lightProjection =
        glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
    return lightProjection * lightView;
}

glm::vec3 Light::getDirection() const { return owner()->getFront(); }

glm::vec3 Light::getPosition() const { return owner()->getPosition(); }

}  // namespace te
