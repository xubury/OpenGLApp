#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <Editor/EditorContext.hpp>
#include <Graphic/Vertex.hpp>

namespace te {

struct Axes {
    glm::vec3 origin;
    glm::vec3 axes[3];
};

class Editor {
   public:
    static Editor &instance();

    void render();

    void close();

    EditorContext context;

   private:
    Editor();

    // build the model's axes coordinate
    void buildModelAxes(float clipLen);

    void renderFps();

    void renderBoundingBox();

    void renderModelAxes();

    void renderCameraAxes(float clipLen);

    void computeTranslateType();

    void handleMouseLeftButton();

    void handleMouseRightButton();

    glm::vec3 m_camRayOrigin;

    glm::vec3 m_camRayDir;

    Axes m_modelAxes;

    glm::vec3 m_modelQuads[3][4];

    bool m_leftMouseDown;

    bool m_rightMouseDown;

    glm::vec2 m_mouseClickPos;

    glm::vec3 m_intersectWorldPos;

    glm::vec3 m_rotationVector;

    enum MoveType {
        NONE = 0,
        TRANSLATE_X,
        TRANSLATE_Y,
        TRANSLATE_Z,
        TRANSLATE_YZ,
        TRANSLATE_XZ,
        TRANSLATE_XY,
        TRANSLATE_XYZ,
        ROTATE_X,
        ROTATE_Y,
        ROTATE_Z
    };

    MoveType m_moveType;

    glm::vec4 m_movePlane;

    float m_axisSizeFactor;
};

}  // namespace te

#endif /* EDITOR_HPP */
