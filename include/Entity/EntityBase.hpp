#ifndef ENTITY_BASE_HPP
#define ENTITY_BASE_HPP

#include <ECS/Entity.hpp>
#include <Graphic/TextureArray.hpp>

namespace te {

class EntityBase : public Entity<EntityBase>, public Drawable {
   public:
    EntityBase(const EntityBase &) = delete;

    EntityBase &operator=(const EntityBase &) = delete;

    virtual ~EntityBase() = default;

    EntityBase(EntityManager<EntityBase> *manager, uint32_t id);

    void setPosition(const glm::vec3 &position);

    glm::vec3 getPosition() const;

    void setEulerAngle(const glm::vec3 &angle);

    glm::vec3 getEulerAngle() const;

    std::string getName() const;

    void setName(const std::string &name);

    void setTextures(const TextureArray &textures);

    const TextureArray *getTextures() const;

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    std::string m_name;
    TextureArray m_textures;
};

}  // namespace te

#endif
