#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <ResourceManager.hpp>
#include <Window/RenderWindow.hpp>
#include <Graphic/Shader.hpp>
#include <ECS/ECS.hpp>
#include <Utility/Time.hpp>
#include <ECS/Application.hpp>
#include <Graphic/FrameBuffer.hpp>

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

    void addSphere(const glm::vec3 &pos, const TextureArray &textures);

    void addCube(const glm::vec3 &pos, float width, float height, float length,
                 const TextureArray &textures);

    void addModel(const std::string &path, const glm::vec3 &pos);

   private:
    RenderWindow m_window;
    Shader m_shader;
    Shader m_shadowShader;
    Shader m_fbShader;
    Application<EntityBase> m_app;
    EntityManager<EntityBase> m_cameras;
    uint32_t m_activeCam;
    FrameBuffer m_frameBuffer;

    bool m_editorMode;
};

#endif
