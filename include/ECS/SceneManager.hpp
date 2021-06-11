#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "ECS/EntityManager.hpp"
#include "ECS/System.hpp"

namespace te {

template <typename ENTITY>
class SceneManager {
   public:
    SceneManager();
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

    void update(const Time &deltaTime);

    SystemManager<ENTITY> systems;
    EntityManager<ENTITY> entities;
};

template <typename ENTITY>
SceneManager<ENTITY>::SceneManager() : systems(entities) {}

template <typename ENTITY>
void SceneManager<ENTITY>::update(const Time &deltaTime) {
    systems.updateAll(deltaTime);
    entities.update();
}

}  // namespace te

#endif
