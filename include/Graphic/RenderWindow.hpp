#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <queue>
#include <string>

#include <Window/GlContext.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Window/Event.hpp>
#include <Utility/Time.hpp>

class RenderWindow : public GlContext, public RenderTarget {
   public:
    RenderWindow(int width, int height, const std::string& title);

    ~RenderWindow();

    bool pollEvent(Event& event);

    bool shouldClose();

    void setShouldClose(bool close = true);

    void display();

    void close();

    void setFramerateLimit(uint32_t fps);

   private:
    friend class GlContext;

    void pollEvents();

    void pushEvent(const Event& event);

    bool popEvent(Event& event, bool block);

    std::queue<Event> m_events;

    Time m_framerateLimit;
    Clock m_clock;
};

#endif
