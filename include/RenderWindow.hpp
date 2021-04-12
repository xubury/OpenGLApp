#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <string>

#include "Event.hpp"
#include "RenderTarget.hpp"

class RenderWindow : public RenderTarget {
   public:
    RenderWindow(int width, int height, const std::string& title);
    ~RenderWindow();

    void initialize(int width, int height, const std::string& title);

    void processEvents();

    bool shouldClose();

    void setShouldClose(bool close = true);

    void swapBuffers();

    void close();

    bool pollEvent(Event& event);

   private:
    void pushEvent(const Event& event);

    bool popEvent(Event& event, bool block);

    static void errorCallback(int error, const char* description);

    static void framebufferSizeCB(GLFWwindow* window, int width, int height);

    static void keyCallback(GLFWwindow* window, int key, int scanCode,
                            int action, int mods);

    GLFWwindow* m_window;
    std::queue<Event> m_events;
};

#endif
