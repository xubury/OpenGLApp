#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <ResourceManager.hpp>
#include <Graphic/RenderWindow.hpp>
#include <Graphic/Shader.hpp>
#include <ECS/ECS.hpp>
#include <Time.hpp>
#include <ECS/Application.hpp>

class Game {
   public:
    Game(int width, int height, const std::string &title);

    void run(int minFps);

    void update(Time &deltaTime);

    void render();

    void addCube(const glm::vec3 &pos, const TextureArray &textures);

    void addModel(const std::string &path);

   private:
    RenderWindow m_window;
    Shader m_shader;
    Application<EntityBase> m_app;
};

#endif