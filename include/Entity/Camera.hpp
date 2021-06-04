#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <Entity/EntityBase.hpp>
#include <Window/ActionTarget.hpp>
#include <Component/Transform.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Graphic/CameraBase.hpp>

namespace te {

inline const float SPEED = 2.5f;
inline const float MOUSE_SENSITIVITY = 0.1f;

enum Movement { FORWARD, BACKWRAD, UPWARD, DOWNWARD, LEFT, RIGHT };

class Camera : public CameraBase,
               public Transform,
               protected ActionTarget<Movement> {
   public:
    using ActionTarget::processEvent;
    using ActionTarget::processEvents;
    using ActionTarget::setActive;

   public:
    Camera(int x, int y, int width, int height,
           const glm::vec3 &position = glm::vec3(0.f));

    virtual glm::mat4 getView() const override;

    virtual glm::mat4 getProjection() const override;

    void move(Movement dir, float val);

    void rotate(float yaw, float pitch, bool constraintPitch = true);

    void zoom(float zoom);

    float getFOV() const;

   protected:
    static ActionMap<Movement> s_cameraMovement;

   private:
    float m_zoom;
    float m_yaw;
    float m_pitch;
};

}

#endif
