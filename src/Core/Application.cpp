#include "Core/Application.hpp"
#include "Graphic/Renderer.hpp"

namespace te {

Application::Application(const Settings &settings)
    : m_window(settings.width, settings.height, settings.title,
               settings.samples) {
    m_window.setFramerateLimit(settings.frameRateLimit);
    m_imGuiLayer = createRef<EditorLayer>();
    m_imGuiLayer->context.setWindow(&m_window);
    m_mainCamera = m_imGuiLayer->getCamera();
    pushOverlay(m_imGuiLayer);
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
            }
            for (auto &layer : m_layers) {
                layer->onEventPoll(event);
            }
        }
        for (auto &layer : m_layers) {
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
    m_imGuiLayer->close();
    m_window.close();
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
