#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include <ResourceManager.hpp>
#include <Graphic/RenderWindow.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/Cube.hpp>

class Application {
   public:
    Application(int width, int height, const std::string &title);

    void run();

    void update();

    void render();

   private:
    RenderWindow m_window;
    Shader m_shader;
    Cube m_cube1;
    Cube m_cube2;
    std::unordered_map<std::string, TextureArray> m_textureManager;
};

#endif
