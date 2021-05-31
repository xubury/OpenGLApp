#include <Graphic/RenderStates.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/Texture.hpp>
#include <Graphic/CameraBase.hpp>

const RenderStates RenderStates::Default;

RenderStates::RenderStates()
    : shader(nullptr),
      transform(1.0f),
      textures(nullptr),
      camera(nullptr),
      light(nullptr),
      depthMapDraw(false),
      depthMapTexture(0) {}
