#include <Entity/Cube.hpp>

#include <Graphic/Shader.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Graphic/RenderStates.hpp>

#include <Component/Transform.hpp>
#include <Component/BoundingBox.hpp>

VertexBuffer Cube::s_cube;

Cube::Cube(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    Vertex vertices[] = {
        {{-0.5f, -0.5f, -0.5f},
         glm::vec4(0),
         {0.0f, 0.0f},
         {0.0f, 0.0f, -1.0f}},
        {{0.5f, -0.5f, -0.5f}, glm::vec4(0), {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, 0.5f, -0.5f}, glm::vec4(0), {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.5f, 0.5f, -0.5f}, glm::vec4(0), {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f, 0.5f, -0.5f}, glm::vec4(0), {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f, -0.5f, -0.5f},
         glm::vec4(0),
         {0.0f, 0.0f},
         {0.0f, 0.0f, -1.0f}},
        {{-0.5f, -0.5f, 0.5f}, glm::vec4(0), {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0, 1.0}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0, 1.0}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0, 1.0}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0, 1.0}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, glm::vec4(0), {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, glm::vec4(0), {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, glm::vec4(0), {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},
         glm::vec4(0),
         {0.0f, 1.0f},
         {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},
         glm::vec4(0),
         {0.0f, 1.0f},
         {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, glm::vec4(0), {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, glm::vec4(0), {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, glm::vec4(0), {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, glm::vec4(0), {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, glm::vec4(0), {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, glm::vec4(0), {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, glm::vec4(0), {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, glm::vec4(0), {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},
         glm::vec4(0),
         {0.0f, 1.0f},
         {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, glm::vec4(0), {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, glm::vec4(0), {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, glm::vec4(0), {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, glm::vec4(0), {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f},
         glm::vec4(0),
         {0.0f, 1.0f},
         {0.0f, -1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, glm::vec4(0), {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, glm::vec4(0), {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, glm::vec4(0), {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, glm::vec4(0), {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, glm::vec4(0), {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, glm::vec4(0), {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}};

    if (!s_cube.isInit()) {
        s_cube.initialize();
        s_cube.update(vertices, 36, GL_TRIANGLES, GL_STATIC_DRAW);
    }
    manager->addComponent<BoundingBox>(id)->initialize(vertices, 36);
}

void Cube::setTextures(const TextureArray &textures) { m_textures = textures; }

void Cube::draw(RenderTarget &target, RenderStates states) const {
    states.transform = component<Transform>()->getMatrix();
    states.textures = &m_textures;
    target.draw(s_cube, states);
}
