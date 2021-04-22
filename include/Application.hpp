#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <string>
#include <ResourceManager.hpp>
#include <Graphic/RenderWindow.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/Model.hpp>
#include <Graphic/Cube.hpp>

class Application {
   public:
    Application(int width, int height, const std::string &title);

    void run();

    void update();

    void render();

   private:
    RenderWindow m_window;
    Model m_obj;
    Shader m_shader;
    Cube m_cube[10];
};

#endif
