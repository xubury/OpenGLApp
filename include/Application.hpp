#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include "ResourceManager.hpp"
#include "Graphic/RenderWindow.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/Texture.hpp"
#include "Graphic/Cube.hpp"

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
