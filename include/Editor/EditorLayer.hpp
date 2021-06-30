#ifndef EDITOR_LAYER_HPP
#define EDITOR_LAYER_HPP

#include "ECS/ECS.hpp"
#include "Entity/EntityBase.hpp"
#include "Editor/EditorContext.hpp"
#include "Editor/EditorCamera.hpp"
#include "Core/Layer.hpp"

namespace te {

struct Axes {
    glm::vec3 origin;
    glm::vec3 axes[3];
};

class EditorLayer : public Layer {
   public:
    EditorLayer(int samples);

    void onAttach() override;

    void onDetech() override;

    void onUpdate(const Time &deltaTime) override;

    void onImGuiRender() override;

    void onEventPoll(const Event &event) override;

    void onEventProcess() override;

    EditorContext context;

    void begin();

    void end();

    Ref<Camera> getCamera() { return m_camera; }

   private:
    EntityBase *getActiveEntityPtr();

    // return the size of clipSize in world space
    float getClipSizeInWorld(float clipSize) const;

    // build the model's axes coordinate
    void buildModelAxes(float clipLen);

    void renderFps();

    void renderBoundingBox();

    void renderCollider();

    void renderModelAxes();

    void renderCameraAxes(float clipLen);

    void computeTranslateType();

    void handleMouseLeftButton();

    void handleMouseRightButton();

    Ref<FrameBuffer> m_frameBuffer;

    Ref<FrameBuffer> m_multiSampleFramebuffer;

    int m_width;

    int m_height;

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

    Ref<EditorCamera> m_camera;

    float m_screenFactor;

    glm::vec2 m_renderOrigin;

    uint32_t m_activeEntityId;
};

}  // namespace te

#endif /* EDITOR_HPP */
