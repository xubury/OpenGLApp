#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include "Window/ActionTarget.hpp"
#include "Graphic/Camera.hpp"

namespace te {

inline const float SPEED = 2.5f;

enum Movement { FORWARD, BACKWRAD, UPWARD, DOWNWARD, LEFT, RIGHT };

class EditorCamera : public Camera, protected ActionTarget<Movement> {
   public:
    using ActionTarget::processEvent;
    using ActionTarget::processEvents;

   public:
    EditorCamera(int x, int y, int width, int height);

    void move(Movement dir, float val);

    void rotate(float yaw, float pitch, bool constraintPitch = true);

    void zoom(float zoom);

   protected:
    static ActionMap<Movement> s_cameraMovement;

   private:
    float m_yaw;
    float m_pitch;
};

}  // namespace te

#endif
