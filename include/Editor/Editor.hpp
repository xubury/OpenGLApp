#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <Editor/RenderContext.hpp>
#include <Graphic/Vertex.hpp>

struct Axes {
    glm::vec3 origin;
    glm::vec3 axes[3];
};

class Editor {
   public:
    static Editor &instance();

    void render();

    void close();

    RenderContext context;

   private:
    Editor();

    // build the model's axes coordinate
    void buildModelAxes(float clipLen);

    void renderFps();

    void renderModelAxes();

    void renderCameraAxes(float clipLen);

    void computeMoveType();

    void handleTranslation();

    void handleCameraRotation();

    glm::vec3 m_camRayOrigin;

    glm::vec3 m_camRayDir;

    Axes m_modelScreenAxes;

    glm::vec3 m_modelScreenQuads[3][4];

    bool m_leftMouseDown;

    bool m_rightMouseDown;

    glm::vec2 m_lastMousePos;

    glm::vec3 m_intersectWorldPos;

    enum MoveType : uint8_t {
        NONE = 0,
        TRANSLATE_X = 1,
        TRANSLATE_Y = 2,
        TRANSLATE_Z = 3,
        TRANSLATE_YZ,
        TRANSLATE_XZ,
        TRANSLATE_XY,
        TRANSLATE_XYZ
    };

    MoveType m_moveType;

    glm::vec4 m_movePlane;

    float m_axisSizeFactor;
};

#endif /* EDITOR_HPP */
