#ifndef RENDER_STATES_HPP
#define RENDER_STATES_HPP

#include <glm/glm.hpp>

class Shader;

class Texture;

class Camera;

class RenderStates {
   public:
    RenderStates();

    void setTransform(const glm::mat4 &transform);

    void setShader(const Shader &shader);

    void setTexture(const Texture &texture);

    void setCamera(const Camera &projectionView);

    void setupShader() const;

    static const RenderStates Default;

   private:
    void setupTranform() const;

    void setupTexture() const;

    void setupCamera() const;

   private:
    const Shader *m_shader;
    glm::mat4 m_transform;
    const Texture *m_texture;
    const Camera *m_camera;
};

#endif
