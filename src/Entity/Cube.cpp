#include <Entity/Cube.hpp>

#include "Graphic/Shader.hpp"
#include "Graphic/Vertex.hpp"
#include "Graphic/Renderer.hpp"

#include "Component/BoundingBox.hpp"

namespace te {

Cube::Cube(EntityManager<EntityBase> *manager, uint32_t id, float width,
           float height, float length, Ref<Material> material)
    : EntityBase(manager, id), m_material(material) {
    width /= 2;
    height /= 2;
    length /= 2;
    Vertex vertices[] = {
        {{-width, -height, -length}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{width, -height, -length}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{width, height, -length}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{width, height, -length}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-width, height, -length}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{-width, -height, -length}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{-width, -height, length}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{width, -height, length}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{width, height, length}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{width, height, length}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-width, height, length}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-width, -height, length}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-width, height, length}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-width, height, -length}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-width, -height, -length}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-width, -height, -length}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-width, -height, length}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-width, height, length}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{width, height, length}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{width, height, -length}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{width, -height, -length}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{width, -height, -length}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{width, -height, length}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{width, height, length}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-width, -height, -length}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{width, -height, -length}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{width, -height, length}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{width, -height, length}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{-width, -height, length}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{-width, -height, -length}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{-width, height, -length}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{width, height, -length}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{width, height, length}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{width, height, length}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-width, height, length}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-width, height, -length}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}}};

    manager->addComponent<BoundingBox>(id)->initialize(vertices, 36);

    m_cube = createRef<VertexArray>();
    Ref<VertexBuffer> vertexBuffer =
        createRef<VertexBuffer>(vertices, 36 * sizeof(Vertex));
    vertexBuffer->setLayout({{ShaderDataType::Float3, "aPos"},
                             {ShaderDataType::Float2, "aTexCoord"},
                             {ShaderDataType::Float3, "aNormal"},
                             {ShaderDataType::Int4, "aBoneIds"},
                             {ShaderDataType::Float4, "aWeights"}});
    m_cube->addVertexBuffer(vertexBuffer);
}

void Cube::draw(const Shader &shader, const glm::mat4 &transform) const {
    Renderer::submit(shader, *m_cube, GL_TRIANGLES, false,
                     getTransform() * transform, m_material.get());
}

}  // namespace te
