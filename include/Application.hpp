#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include "ResourceManager.hpp"
#include "RenderWindow.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Cube.hpp"
#include "Camera.hpp"

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
    ResourceManager<Texture, std::string> m_textureManager;
};

#endif
