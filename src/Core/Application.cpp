#include "Core/Application.hpp"
#include "Graphic/Renderer.hpp"
#include "Graphic/GLContext.hpp"

namespace te {

Application *Application::s_instance = nullptr;

Application::Application(const Settings &settings)
    : m_window(settings.width, settings.height, settings.title,
               settings.samples) {
    m_window.setFramerateLimit(settings.frameRateLimit);
    m_imGuiLayer = createRef<EditorLayer>();
    m_mainCamera = createRef<Camera>(0, 0, settings.width, settings.height);
    m_mainCamera->setPosition(glm::vec3(-8.f, 15.f, 21.f));
    m_mainCamera->setEulerAngle(glm::vec3(
        glm::radians(-25.f), glm::radians(-28.f), glm::radians(1.5f)));
    pushOverlay(m_imGuiLayer);
    s_instance = this;
}

void Application::pushLayer(Ref<Layer> layer) { m_layers.pushLayer(layer); }

void Application::pushOverlay(Ref<Layer> overlay) {
    m_layers.pushOverlay(overlay);
}

void Application::run(int minFps) {
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;
    Time timePerFrame = seconds(1.0 / minFps);
    while (!m_window.shouldClose()) {
        Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == Event::KEY_PRESSED &&
                event.key.code == Keyboard::ESCAPE) {
                m_window.setShouldClose(true);
                break;
            } else if (event.type == Event::RESIZED) {
                m_mainCamera->setViewportSize(event.size.width,
                                              event.size.height);
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
    for (auto &layer : m_layers) {
        layer->onUpdate(deltaTime);
    }
}

void Application::render() {
    m_imGuiLayer->begin();
    Renderer::beginScene(*m_mainCamera);
    for (auto &layer : m_layers) {
        layer->onRender();
    }

    for (auto &layer : m_layers) {
        layer->onImGuiRender();
    }
    Renderer::endScene();
    m_imGuiLayer->end();
    m_window.display();
}

}  // namespace te
