#ifndef CAMERA_COMP_HPP
#define CAMERA_COMP_HPP

#include "ECS/ECS.hpp"
#include "Graphic/Camera.hpp"
#include "Entity/EntityBase.hpp"

namespace te {

class CameraComp : public Component<CameraComp, EntityBase>, public Camera {
   public:
    CameraComp(int x, int y, int width, int height);

    bool isPrimary() const { return m_primary; }

    void setPrimary(bool primary) { m_primary = primary; }

   private:
    bool m_primary;
};

}  // namespace te

#endif /* CAMERA_COMP_HPP */
