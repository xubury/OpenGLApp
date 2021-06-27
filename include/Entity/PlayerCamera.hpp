#ifndef PLAYER_CAMERA_HPP
#define PLAYER_CAMERA_HPP

#include "ECS/ECS.hpp"
#include "Graphic/Camera.hpp"
#include "Window/ActionTarget.hpp"

namespace te {

class Player;

class PlayerCamera : public EntityBase,
                     public Camera,
                     protected ActionTarget<int> {
   public:
    using ActionTarget::processEvent;
    using ActionTarget::processEvents;

   public:
    ActionMap<int> m_cameraActionMap;

    PlayerCamera(EntityManager<EntityBase> *manager, uint32_t id, int x, int y,
                 int width, int height, Player *player);

    void update(const Time &deltaTime) override;

    void rotate(float pitch, float yaw);

   private:
    bool m_isFirstMouse;
    glm::vec2 m_lastMousePos;
    Player *m_player;
    float m_dist;
    float m_yaw;
    float m_pitch;
};

}  // namespace te

#endif /* PLAYER_CAMERA_HPP */
