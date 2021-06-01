#ifndef LIGHT_BASE_HPP
#define LIGHT_BASE_HPP

#include <glm/glm.hpp>
#include <Graphic/ShadowBuffer.hpp>
#include <Graphic/OpenGL.hpp>

class LightBase {
   public:
    enum LightType { DIRECTIONAL, POINT };
    virtual ~LightBase() = default;

    virtual glm::mat4 getViewMatirx() const = 0;
    virtual glm::vec3 getDirection() const = 0;
    virtual glm::vec3 getPosition() const = 0;
    glm::vec3 amibent;
    glm::vec3 diffuse;
    glm::vec3 specular;

    const ShadowBuffer &getShadowBuffer() const;

   private:
    ShadowBuffer m_shadowBuffer;
};

#endif /* LIGHT_BASE_HPP */
