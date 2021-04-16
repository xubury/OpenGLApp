#include "Graphic/RenderStates.hpp"

#include <glad/glad.h>

#include "Graphic/Shader.hpp"
#include "Graphic/Texture.hpp"
#include "Graphic/Camera.hpp"

RenderStates::RenderStates()
    : shader(nullptr), transform(1.0f), texture(nullptr), camera(nullptr) {}

const RenderStates RenderStates::Default;
