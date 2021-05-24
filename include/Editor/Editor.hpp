#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <Editor/RenderContext.hpp>

struct Axis {
    glm::vec3 pos;
    uint32_t color;
};

struct Axes {
    glm::vec3 origin;
    Axis axes[3];
};

class Editor {
   public:
    static Editor &instance();

    void render();

    void close();

    RenderContext context;

   private:
    Editor();

    // build the model's axes coordinate and color
    void buildModelAxes(float clipLen);

    // build model's X-Y X-Z Y-Z plane
    void buildModelPlane();

    void renderFps();

    void renderModelAxes();

    void renderCameraAxes(float clipLen);

    glm::vec3 m_camRayOrigin;

    glm::vec3 m_camRayDir;

    Axes m_modelScreenAxes;

    int m_axesDrawingOrder[3];

    glm::vec4 m_planeXY;

    glm::vec4 m_planeXZ;

    glm::vec4 m_planeYZ;
};

#endif /* EDITOR_HPP */
