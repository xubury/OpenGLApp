#include "Apps/Application.hpp"
#include "Graphic/Renderer.hpp"
#include "Graphic/GLContext.hpp"

namespace te {

Application *Application::s_instance = nullptr;

Application::Application(const Settings &settings)
    : m_window(settings.width, settings.height, settings.title,
               settings.samples),
      m_mainCamera(nullptr),
      m_editorMode(settings.editor) {
    s_instance = this;
    m_window.setFramerateLimit(settings.frameRateLimit);
    m_imGuiLayer = createRef<EditorLayer>(settings.samples);
    Renderer::init();
    if (m_editorMode) {
        pushOverlay(m_imGuiLayer);
    } else {
        popOverlay(m_imGuiLayer);
    }

    m_scene = createRef<SceneManager<EntityBase>>();
}

void Application::pushLayer(Ref<Layer> layer) { m_layers.pushLayer(layer); }

void Application::pushOverlay(Ref<Layer> overlay) {
    m_layers.pushOverlay(overlay);
}
void Application::popLayer(Ref<Layer> layer) { m_layers.popLayer(layer); }

void Application::popOverlay(Ref<Layer> overlay) {
    m_layers.popOverlay(overlay);
}

void Application::run(int minFps) {
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;
    Time timePerFrame = seconds(1.0 / minFps);
    while (!m_window.shouldClose()) {
        Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == Event::KEY_PRESSED) {
                if (event.key.code == Keyboard::ESCAPE) {
                    m_window.setShouldClose(true);
                    break;
                }
            }
            for (auto &layer : m_layers) {
                if (layer->isBlockEvent()) continue;
                layer->onEventPoll(event);
            }
        }
        for (auto &layer : m_layers) {
            if (layer->isBlockEvent()) continue;
            layer->onEventProcess();
        }
        timeSinceLastUpdate = clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            update(timePerFrame);
        }
        update(timeSinceLastUpdate);
        render();
    }
}

void Application::update(const Time &deltaTime) {
    m_scene->update(deltaTime);
    for (auto &layer : m_layers) {
        layer->onUpdate(deltaTime);
    }
}

void Application::render() {
    if (m_editorMode) {
        m_imGuiLayer->begin();
    }
    for (auto &layer : m_layers) {
        layer->onRender();
    }
    if (m_editorMode) {
        for (auto &layer : m_layers) {
            layer->onImGuiRender();
        }
    }
    if (m_editorMode) {
        m_imGuiLayer->end();
    }
    m_window.display();
}

void Application::toggleEditor() {
    m_editorMode = !m_editorMode;
    if (m_editorMode) {
        pushOverlay(m_imGuiLayer);
    } else {
        popOverlay(m_imGuiLayer);
    }
}

}  // namespace te
