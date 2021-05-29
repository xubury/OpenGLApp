#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <Entity/EntityBase.hpp>
#include <Window/ActionTarget.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Graphic/CameraBase.hpp>

inline const float SPEED = 2.5f;
inline const float MOUSE_SENSITIVITY = 0.1f;

enum Movement { FORWARD, BACKWRAD, UPWARD, DOWNWARD, LEFT, RIGHT };

class Camera : public CameraBase,
               public EntityBase,
               protected ActionTarget<Movement> {
   public:
    using ActionTarget::processEvent;
    using ActionTarget::processEvents;
    using ActionTarget::setActive;

   public:
    Camera(EntityManager<EntityBase> *manager, uint32_t id, int x, int y,
           int width, int height, const glm::vec3 &position = glm::vec3(0.f));

    virtual void draw(RenderTarget &target, RenderStates states) const override;

    virtual glm::mat4 getView() const override;

    void move(Movement dir, float val);

    void rotate(float yaw, float pitch, bool constraintPitch = true);

   protected:
    static ActionMap<Movement> s_cameraMovement;

   private:
    float m_yaw;
    float m_pitch;
};

class ControlCamera : public Camera {
   public:
    ControlCamera(EntityManager<EntityBase> *manager, uint32_t id, int x, int y,
                  int width, int height,
                  const glm::vec3 &position = glm::vec3(0.f));

   private:
    glm::vec2 m_lastMousePos;
    bool m_isFirstMouse;
};

#endif
