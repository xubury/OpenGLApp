#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <ECS/EntityManager.hpp>
#include <ECS/System.hpp>

template <typename ENTITY>
class Application {
   public:
    Application();
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void update(const Time &deltaTime);

    SystemManager<ENTITY> systems;
    EntityManager<ENTITY> entities;
};

template <typename ENTITY>
Application<ENTITY>::Application() : systems(entities) {
    entities.setSystems(&systems);
}

template <typename ENTITY>
void Application<ENTITY>::update(const Time &deltaTime) {
    systems.updateAll(deltaTime);
    entities.update();
}

#endif
