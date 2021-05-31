#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <Entity/EntityBase.hpp>
#include <Component/Transform.hpp>
#include <Graphic/LightBase.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

class Light : public EntityBase, public LightBase {
   public:
    Light(EntityManager<EntityBase> *manager, uint32_t id)
        : EntityBase(manager, id) {}

    glm::mat4 getViewMatirx() const override {
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
    glm::vec3 getDirection() const override {
        return component<Transform>()->getFront();
    }

    glm::vec3 getPosition() const override {
        return component<Transform>()->getPosition();
    }

    void draw(RenderTarget &, RenderStates) const override {}
};

#endif /* LIGHT_HPP */
