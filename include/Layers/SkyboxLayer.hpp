#ifndef SKYBOX_LAYER_HPP
#define SKYBOX_LAYER_HPP

#include "Core/Layer.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Cubemap.hpp"

namespace te {

class SkyboxLayer : public Layer {
   public:
    SkyboxLayer();

    void onRender();

   private:
    Ref<Shader> m_shader;
    Ref<VertexArray> m_vertexArray;
    Cubemap m_cubemap;
};
}  // namespace te

#endif /* SKYBOX_LAYER_HPP */
