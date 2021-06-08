#ifndef GAME_HPP
#define GAME_HPP

#include <string>

#include "Window/RenderWindow.hpp"
#include "Entity/Camera.hpp"
#include "Graphic/Shader.hpp"
#include "ECS/ECS.hpp"
#include "Core/Time.hpp"
#include "ECS/Application.hpp"
#include "Graphic/FrameBuffer.hpp"

using namespace te;

struct Settings {
    int width;
    int height;
    std::string title;
    int samples;
    bool editor;
    int frameRateLimit;
};

class Game {
   public:
    Game(const Settings &settings);

    void run(int minFps);

    void update(Time &deltaTime);

    void render();

    void addSphere(const glm::vec3 &pos, const glm::vec3 &impulse,
                   const TextureArray &textures);

    void addCube(const glm::vec3 &pos, float width, float height, float length,
                 const TextureArray &textures, bool kinematic);

    void addModel(const std::string &path, const glm::vec3 &pos);

   private:
    void loadShaders();

    void loadScene();

   private:
    RenderWindow m_window;
    ShaderLibrary m_shaders;
    Application<EntityBase> m_app;
    Ref<Camera> m_mainCamera;
    FrameBuffer m_frameBuffer;

    bool m_editorMode;
};

#endif
