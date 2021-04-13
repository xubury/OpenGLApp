#include "RenderStates.hpp"

#include <glad/glad.h>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

RenderStates::RenderStates() : transform(1.0f) {}

const RenderStates RenderStates::Default;
