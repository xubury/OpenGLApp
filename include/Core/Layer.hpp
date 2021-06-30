#ifndef LAYER_HPP
#define LAYER_HPP

#include <string>
#include "Core/Time.hpp"
#include "Window/Event.hpp"

namespace te {

class TE_API Layer {
   public:
    Layer(const std::string &name) : m_name(name), m_blockEvent(false) {}

    virtual ~Layer() = default;

    Layer(const Layer &) = delete;

    Layer &operator=(const Layer &) = delete;

    virtual void onAttach() {}

    virtual void onDetech() {}

    virtual void onUpdate(const Time &) {}

    virtual void onRender() {}

    virtual void onImGuiRender() {}

    virtual void onEventPoll(const Event &) {}

    virtual void onEventProcess() {}

    void setBlockEvent(bool block) { m_blockEvent = block; }

    bool isBlockEvent() const { return m_blockEvent; }

   protected:
    std::string m_name;
    bool m_blockEvent;
};
}  // namespace te

#endif /* LAYER_HPP */
