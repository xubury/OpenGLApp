#include <Entity/Cube.hpp>

#include <Graphic/Shader.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Graphic/RenderStates.hpp>

#include <Component/Transform.hpp>
#include <Component/BoundingBox.hpp>

Cube::Cube(EntityManager<EntityBase> *manager, uint32_t id, float width,
           float height, float length)
    : EntityBase(manager, id) {
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

    s_cube.initialize();
    s_cube.update(vertices, 36, GL_TRIANGLES, GL_STATIC_DRAW);
    manager->addComponent<BoundingBox>(id)->initialize(vertices, 36);
}

void Cube::draw(RenderTarget &target, RenderStates states) const {
    target.draw(s_cube, states);
}
