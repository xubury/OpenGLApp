#include "Entity/Player.hpp"
#include "Graphic/Renderer.hpp"
#include "Physics/Rigidbody.hpp"
#include "Physics/HullCollider.hpp"
#include "Component/Controller.hpp"

namespace te {

Player::Player(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    setName("Player");
    float width = 1;
    float height = 1;
    float length = 1;
    m_model = createRef<Model>();
    m_model->loadFromFile("resources/models/vampire/dancing_vampire.dae");

    m_animation = createRef<Animation>(
        "resources/models/vampire/dancing_vampire.dae", *m_model);
    m_animator = createRef<Animator>(m_animation.get());

    add<Rigidbody>(100, true);
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

void Player::update(const Time &deltaTime) { m_animator->update(deltaTime); }

void Player::draw(const Shader &shader) const {
    shader.bind();
    auto &transforms = m_animator->getFinalBoneMatrices();
    for (std::size_t i = 0; i < transforms.size(); ++i) {
        shader.setMat4("uBoneTransform[" + std::to_string(i) + "]",
                       transforms[i]);
    }
    m_model->draw(shader, getTransform());
}

void Player::move(Action movement) {
    Rigidbody *rigidbody =
        dynamic_cast<Rigidbody *>(component<CollisionObject>().get());

    glm::vec3 front = m_camera->getFront();
    front.y = 0.f;
    front = glm::normalize(-front);
    glm::vec3 left = m_camera->getLeft();
    left.y = 0.f;
    left = glm::normalize(-left);

    float amplifier = 20.f;
    if (movement == Action::MOVE_JUMP) {
        rigidbody->addForce(
            glm::vec3(0.f, 1.0f, 0.f) * rigidbody->getMass() * 12.0f,
            glm::vec3(0));
    } else if (movement == Action::MOVE_FORWARD) {
        rigidbody->addImpulse(front * amplifier);
    } else if (movement == Action::MOVE_BACKWARD) {
        rigidbody->addImpulse(-front * amplifier);
    } else if (movement == Action::MOVE_LEFT) {
        rigidbody->addImpulse(left * amplifier);
    } else if (movement == Action::MOVE_RIGHT) {
        rigidbody->addImpulse(-left * amplifier);
    }

    float acos = std::acos(glm::dot(getFront(), front));
    glm::vec3 c = glm::cross(getFront(), front);
    if (c.y < 0) {
        rotateLocal(-0.1f * acos, glm::vec3(0, 1.0, 0));
    } else {
        rotateLocal(0.1f * acos, glm::vec3(0, 1.0, 0));
    }
}

}  // namespace te
