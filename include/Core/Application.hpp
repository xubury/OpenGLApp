
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include "Core/Base.hpp"
#include "Core/LayerStack.hpp"
#include "Window/RenderWindow.hpp"
#include "Editor/EditorLayer.hpp"

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
    Application(const Settings &settings);

    virtual ~Application() = default;

    Application(const Application &) = delete;

    Application &operator=(const Application &) = delete;

    void pushLayer(Ref<Layer> layer);

    void pushOverlay(Ref<Layer> overlay);

    void run(int minFps);

    Ref<EditorLayer> getEditor() { return m_imGuiLayer; };
   private:
    void update(const Time &deltaTime);

    void render();

    RenderWindow m_window;
    LayerStack m_layers;
    Ref<EditorLayer> m_imGuiLayer;
    Ref<CameraBase> m_mainCamera;
};

}  // namespace te

#endif /* APPLICATION_HPP */
