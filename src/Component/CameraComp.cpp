#include "Component/CameraComp.hpp"

namespace te {

CameraComp::CameraComp(int x, int y, int width, int height)
    : Camera(x, y, width, height), m_primary(true) {}

}  // namespace te
