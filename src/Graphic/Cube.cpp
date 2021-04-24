#include <glad/glad.h>

#include <Graphic/Cube.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Graphic/RenderStates.hpp>

#include <Component/Transform.hpp>
#include <Component/BoundingBox.hpp>

VertexBuffer Cube::s_cube(GL_TRIANGLES);

Cube::Cube(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    Vertex vertices[] = {
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}};

    if (s_cube.empty()) {
        s_cube.create(vertices, 36);
    }
    manager->addComponent<BoundingBox>(id)->initialize(vertices, 36);
}

void Cube::setTextures(const TextureArray &textures) { m_textures = textures; }

void Cube::draw(RenderTarget &target, RenderStates states) const {
    states.transform = component<Transform>()->getTransform();
    states.textures = &m_textures;
    s_cube.draw(target, states);
}
