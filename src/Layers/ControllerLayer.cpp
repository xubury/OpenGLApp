#include "Layers/ControllerLayer.hpp"
#include "Apps/Application.hpp"
#include "Component/Controller.hpp"

namespace te {
ControllerLayer::ControllerLayer() : Layer("Controller Layer") {}

void ControllerLayer::onEventPoll(const Event& event) {
    Controller::Handle controller;
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    auto view = scene->entities.getByComponents(controller);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        controller->processEvent(event);
    }
}

void ControllerLayer::onEventProcess() {
    Controller::Handle controller;
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    auto view = scene->entities.getByComponents(controller);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        controller->processEvents();
    }
}

}  // namespace te
