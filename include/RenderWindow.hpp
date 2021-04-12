#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <queue>
#include <string>

#include "GlContext.hpp"
#include "Event.hpp"
#include "RenderTarget.hpp"

class RenderWindow : public GlContext, public RenderTarget {
   public:
    RenderWindow(int width, int height, const std::string& title);

    ~RenderWindow();

    void processEvents();

    bool pollEvent(Event& event);

   private:
    friend class GlContext;

    void pushEvent(const Event& event);

    bool popEvent(Event& event, bool block);

    std::queue<Event> m_events;
};

#endif
