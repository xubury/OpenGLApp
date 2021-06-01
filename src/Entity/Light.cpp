#include <Entity/Light.hpp>

Light::Light(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {}

glm::mat4 Light::getViewMatirx() const {
    auto trans = component<Transform>();
    const glm::vec3 &up = trans->getUp();
    const glm::vec3 &front = trans->getFront();
    const glm::vec3 &pos = trans->getPosition();
    glm::mat4 lightView = glm::lookAt(pos, pos + front, up);
    float near_plane = 1.0f, far_plane = 100.0f;
    glm::mat4 lightProjection =
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    return lightProjection * lightView;
}
glm::vec3 Light::getDirection() const {
    return component<Transform>()->getFront();
}

glm::vec3 Light::getPosition() const {
    return component<Transform>()->getPosition();
}

void Light::draw(RenderTarget &, RenderStates) const {}
