#include "Entity/Player.hpp"
#include "Graphic/Renderer.hpp"
#include "Physics/Rigidbody.hpp"
#include "Physics/HullCollider.hpp"
#include "Component/EventComp.hpp"
#include "Component/PlayerCameraComp.hpp"
#include "Apps/Application.hpp"

namespace te {

inline const float MOUSE_SENSITIVITY = 0.1f;

Player::Player(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id), m_moveDir(getFront()), m_isFirstMouse(true) {
    setName("Player");
    float width = 1;
    float height = 1;
    float length = 1;
    m_model = createRef<Model>();
    m_model->loadFromFile("resources/models/treeman/treeman.dae");

    m_animation =
        createRef<Animation>("resources/models/treeman/treeman.dae", *m_model);
    m_animator = createRef<Animator>(m_animation.get());

    add<Rigidbody>(10, true);
    add<HullCollider>();
    MakeCubeCollider(*component<HullCollider>().get(), width, height, length,
                     glm::vec3(0, 0.5f, 0));

    add<PlayerCameraComp>(0, 0, Application::instance().getWindow().width(),
                          Application::instance().getWindow().height());

    m_inputs.map(PlayerAction::MOVE_JUMP, Keyboard::SPACE);
    m_inputs.map(PlayerAction::MOVE_FORWARD, Keyboard::W);
    m_inputs.map(PlayerAction::MOVE_BACKWARD, Keyboard::S);
    m_inputs.map(PlayerAction::MOVE_LEFT, Keyboard::A);
    m_inputs.map(PlayerAction::MOVE_RIGHT, Keyboard::D);

    add<EventComp>(m_inputs);
    EventComp::Handle evtComp = component<EventComp>();

    evtComp->bind(PlayerAction::MOVE_JUMP,
                  [this](const Event &) { move(PlayerAction::MOVE_JUMP); });
    evtComp->bind(PlayerAction::MOVE_FORWARD,
                  [this](const Event &) { move(PlayerAction::MOVE_FORWARD); });
    evtComp->bind(PlayerAction::MOVE_BACKWARD,
                  [this](const Event &) { move(PlayerAction::MOVE_BACKWARD); });
    evtComp->bind(PlayerAction::MOVE_LEFT,
                  [this](const Event &) { move(PlayerAction::MOVE_LEFT); });
    evtComp->bind(PlayerAction::MOVE_RIGHT,
                  [this](const Event &) { move(PlayerAction::MOVE_RIGHT); });
    evtComp->bind(Action(Event::EventType::MOUSE_MOVED),
                  [this](const Event &event) {
                      glm::vec2 currentMousePos =
                          glm::vec2(event.mouseMove.x, event.mouseMove.y);
                      if (m_isFirstMouse) {
                          m_isFirstMouse = false;
                      } else {
                          glm::vec2 offset = currentMousePos - m_lastMousePos;
                          component<PlayerCameraComp>()->rotate(
                              -offset.y * MOUSE_SENSITIVITY,
                              -offset.x * MOUSE_SENSITIVITY);
                      }
                      m_lastMousePos = currentMousePos;
                  });

    evtComp->bind(Action(Event::EventType::RESIZED),
                  [this](const Event &event) {
                      component<CameraComp>()->setViewportSize(
                          event.size.width, event.size.height);
                  });

    m_timePerMove = seconds(0.1f);
}

void Player::update(const Time &deltaTime) {
    auto cam = component<PlayerCameraComp>();
    cam->setPosition(getPosition() + cam->getFront() * cam->getDistance());

    float acos = std::acos(glm::dot(getFront(), m_moveDir));
    glm::vec3 c = glm::cross(getFront(), m_moveDir);
    if (c.y < 0) {
        rotateLocal(-acos * deltaTime.count() * 2, glm::vec3(0, 1.0, 0));
    } else {
        rotateLocal(acos * deltaTime.count() * 2, glm::vec3(0, 1.0, 0));
    }
    m_animator->update(deltaTime);
}

void Player::draw(const Shader &shader, const glm::mat4 &transform) const {
    shader.bind();
    auto &transforms = m_animator->getFinalBoneMatrices();
    for (std::size_t i = 0; i < transforms.size(); ++i) {
        shader.setMat4("uBoneTransform[" + std::to_string(i) + "]",
                       transforms[i]);
    }
    m_model->draw(shader, getTransform() * transform);
}

void Player::move(PlayerAction movement) {
    if (m_moveClock.getElapsedTime() < m_timePerMove) {
        return;
    }
    m_moveClock.restart();
    Rigidbody *rigidbody =
        dynamic_cast<Rigidbody *>(component<CollisionObject>().get());

    glm::vec3 front = component<CameraComp>()->getFront();
    front.y = 0.f;
    front = glm::normalize(-front);
    glm::vec3 left = component<CameraComp>()->getLeft();
    left.y = 0.f;
    left = glm::normalize(-left);

    float amplifier = 20.f;
    if (movement == PlayerAction::MOVE_JUMP) {
        rigidbody->addForce(
            glm::vec3(0.f, 1.0f, 0.f) * rigidbody->getMass() * amplifier * 10.f,
            glm::vec3(0));
    } else if (movement == PlayerAction::MOVE_FORWARD) {
        rigidbody->addImpulse(front * amplifier);
        m_moveDir = front;
    } else if (movement == PlayerAction::MOVE_BACKWARD) {
        rigidbody->addImpulse(-front * amplifier);
        m_moveDir = -front;
    } else if (movement == PlayerAction::MOVE_LEFT) {
        rigidbody->addImpulse(left * amplifier);
        m_moveDir = left;
    } else if (movement == PlayerAction::MOVE_RIGHT) {
        rigidbody->addImpulse(-left * amplifier);
        m_moveDir = -left;
    }
}

}  // namespace te
