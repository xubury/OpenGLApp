#include "Graphic/RenderStates.hpp"

#include <glad/glad.h>

#include "Graphic/Shader.hpp"
#include "Graphic/Texture.hpp"
#include "Graphic/Camera.hpp"

RenderStates::RenderStates() : transform(1.0f) {}

const RenderStates RenderStates::Default;
