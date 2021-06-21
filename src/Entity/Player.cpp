#include "Entity/Player.hpp"
#include "Graphic/Vertex.hpp"
#include "Graphic/Renderer.hpp"
#include "Physics/Rigidbody.hpp"
#include "Physics/HullCollider.hpp"
#include "Component/Controller.hpp"

namespace te {

Player::Player(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    float width = 1;
    float height = 1;
    float length = 1;
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

    m_vertexArray = createRef<VertexArray>();
    Ref<VertexBuffer> vertexBuffer =
        createRef<VertexBuffer>(vertices, 36 * sizeof(Vertex));
    vertexBuffer->setLayout({{ShaderDataType::Float3, "aPos"},
                             {ShaderDataType::Float2, "aTexCoord"},
                             {ShaderDataType::Float3, "aNormal"}});
    m_vertexArray->addVertexBuffer(vertexBuffer);
    m_material = createRef<Material>();
    m_material->loadFromValue(glm::vec3(1.f), Material::TEXTURE_AMBIENT);
    m_material->loadFromValue(glm::vec3(0.6f), Material::TEXTURE_DIFFUSE);
    m_material->loadFromValue(glm::vec3(0.5f), Material::TEXTURE_SPECULAR);

    add<Rigidbody>(10, true);
    add<HullCollider>();
    MakeCubeCollider(*component<HullCollider>().get(), width, height, length);

    m_inputs.map(Action::MOVE_JUMP, Keyboard::SPACE);
    m_inputs.map(Action::MOVE_FORWARD, Keyboard::W);
    m_inputs.map(Action::MOVE_BACKWARD, Keyboard::S);
    m_inputs.map(Action::MOVE_LEFT, Keyboard::A);
    m_inputs.map(Action::MOVE_RIGHT, Keyboard::D);

    add<Controller>(m_inputs);
    Controller::Handle controller = component<Controller>();

    controller->bind(Action::MOVE_JUMP,
                     [this](const Event &) { move(Action::MOVE_JUMP); });
    controller->bind(Action::MOVE_FORWARD,
                     [this](const Event &) { move(Action::MOVE_FORWARD); });
    controller->bind(Action::MOVE_BACKWARD,
                     [this](const Event &) { move(Action::MOVE_BACKWARD); });
    controller->bind(Action::MOVE_LEFT,
                     [this](const Event &) { move(Action::MOVE_LEFT); });
    controller->bind(Action::MOVE_RIGHT,
                     [this](const Event &) { move(Action::MOVE_RIGHT); });
}

void Player::draw(const Shader &shader) const {
    Renderer::submit(shader, *m_vertexArray, GL_TRIANGLES, false,
                     getTransform(), m_material.get());
}

void Player::move(Action movement) {
    Rigidbody *rigidbody =
        dynamic_cast<Rigidbody *>(component<CollisionObject>().get());
    float amplifier = 5.0f;
    if (movement == Action::MOVE_JUMP) {
        rigidbody->addForce(getUp() * rigidbody->getMass() * 50.f,
                            glm::vec3(0));
    } else if (movement == Action::MOVE_FORWARD) {
        rigidbody->addImpulse(getFront() * amplifier);
    } else if (movement == Action::MOVE_BACKWARD) {
        rigidbody->addImpulse(-getFront() * amplifier);
    } else if (movement == Action::MOVE_LEFT) {
        rigidbody->addImpulse(-getRight() * amplifier);
    } else if (movement == Action::MOVE_RIGHT) {
        rigidbody->addImpulse(getRight() * amplifier);
    }
}

}  // namespace te
