#include <Graphic/RenderStates.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/Texture.hpp>
#include <Graphic/CameraBase.hpp>

const RenderStates RenderStates::Default;

RenderStates::RenderStates() : transform(1.0f), textures(nullptr) {}
