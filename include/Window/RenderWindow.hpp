#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <queue>
#include <string>

#include "Core/Export.hpp"
#include "Window/Event.hpp"
#include "Core/Time.hpp"

class GLFWwindow;

namespace te {

class TE_API RenderWindow {
   public:
    RenderWindow(int width, int height, const std::string& title, int samples);

    ~RenderWindow();

    bool pollEvent(Event& event);

    bool shouldClose();

    void setShouldClose(bool close = true);

    void display();

    void close();

    void setFramerateLimit(uint32_t fps);

    float getFrameRate() const;

    static ::GLFWwindow* getCurrentContext();

    void captureMouse(bool capture);

    uint32_t width() const;

    uint32_t height() const;

   private:
    void pollEvents();

    void pushEvent(const Event& event);

    bool popEvent(Event& event, bool block);

    std::queue<Event> m_events;

    Time m_secondPerFrame;
    Clock m_clock;
    float m_frameRate;
    uint32_t m_width;
    uint32_t m_height;

   private:
    static void errorCallback(int error, const char* description);

    static void framebufferSizeCB(GLFWwindow* window, int width, int height);

    static void keyCallback(GLFWwindow* window, int key, int scanCode,
                            int action, int mod);

    static void mouseMovementCallback(GLFWwindow* window, double x, double y);

    static void mouseButtonCallback(GLFWwindow* window, int button, int type,
                                    int mod);

    static void mouseWheelCallback(GLFWwindow* window, double xOffset,
                                   double yOffset);
};

}  // namespace te

#endif
