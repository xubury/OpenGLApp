#ifndef PLAYER_CAMERA_COMP_HPP
#define PLAYER_CAMERA_COMP_HPP

#include "Component/CameraComp.hpp"

namespace te {

class PlayerCameraComp : public CameraComp {
   public:
    PlayerCameraComp(int x, int y, int width, int height);

    void rotate(float pitch, float yaw);

    float getDistance() const { return m_dist; }

   private:
    float m_dist;
    float m_yaw;
    float m_pitch;
};

}  // namespace te

#endif /* PLAYER_CAMERA_HPP */
