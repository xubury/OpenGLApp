#ifndef RENDER_STATES_HPP
#define RENDER_STATES_HPP

#include <glm/glm.hpp>

class Shader;

class Texture;

class RenderStates {
   public:
    RenderStates();

    void setTransform(const glm::mat4 &transform);

    void setShader(const Shader &shader);

    void setTexture(const Texture &texture);

    void setView(const glm::mat4 &view);

   private:
    friend class RenderTarget;

    void setupShader() const;

    void setupTranform() const;

    void setupTexture() const;

    void setupView() const;

   private:
    const Shader *m_shader;
    glm::mat4 m_transform;
    const Texture *m_texture;
    glm::mat4 m_view;
};

#endif
