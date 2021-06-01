#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <Entity/EntityBase.hpp>
#include <Component/Transform.hpp>
#include <Graphic/LightBase.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

class Light : public EntityBase, public LightBase {
   public:
    Light(EntityManager<EntityBase> *manager, uint32_t id);

    glm::mat4 getLightSpaceMatrix() const override;

    glm::vec3 getDirection() const override;

    glm::vec3 getPosition() const override;

    void draw(RenderTarget &, RenderStates) const override;
};

#endif /* LIGHT_HPP */
