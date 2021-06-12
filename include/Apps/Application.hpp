#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include "Core/Base.hpp"
#include "Core/LayerStack.hpp"
#include "Window/RenderWindow.hpp"
#include "Editor/EditorLayer.hpp"

int main(int argc, char **argv);

namespace te {

struct Settings {
    int width;
    int height;
    std::string title;
    int samples;
    bool editor;
    int frameRateLimit;
};

class Application {
   public:
    static Application &instance() { return *s_instance; }

    Application(const Settings &settings);

    ~Application() = default;

    Application(const Application &) = delete;

    Application &operator=(const Application &) = delete;

    void pushLayer(Ref<Layer> layer);

    void popLayer(Ref<Layer> layer);

    void pushOverlay(Ref<Layer> overlay);

    void popOverlay(Ref<Layer> overlay);

    Ref<EditorLayer> getEditor() { return m_imGuiLayer; };

    RenderWindow &getWindow() { return m_window; }

    void setMainCamera(Ref<Camera> camera) { m_mainCamera = camera; }

    Ref<Camera> getMainCamera() { return m_mainCamera; }

    void setFramebuffer(Ref<FrameBuffer> framebuffer) {
        m_framebuffer = framebuffer;
    }

    Ref<FrameBuffer> getFramebuffer() { return m_framebuffer; }

    void setActiveScene(Ref<SceneManager<EntityBase>> scene) {
        m_activeScene = scene;
    }

    Ref<SceneManager<EntityBase>> getActiveScene() { return m_activeScene; }

    void toggleEditor(bool enable);

   private:
    static Application *s_instance;

    friend int ::main(int argc, char **argv);

    void update(const Time &deltaTime);

    void run(int minFps);

    void render();

    RenderWindow m_window;
    LayerStack m_layers;
    Ref<EditorLayer> m_imGuiLayer;
    Ref<Camera> m_mainCamera;
    Ref<SceneManager<EntityBase>> m_activeScene;
    Ref<FrameBuffer> m_framebuffer;
    bool m_editorMode;
};

}  // namespace te

#endif /* APPLICATION_HPP */
