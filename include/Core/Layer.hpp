#ifndef LAYER_HPP
#define LAYER_HPP

#include <string>
#include "Core/Time.hpp"
#include "Window/Event.hpp"

namespace te {

class Layer {
   public:
    Layer(const std::string &name) : m_name(name) {}

    virtual ~Layer() = default;

    virtual void onAttach() {}

    virtual void onDetech() {}

    virtual void onUpdate(const Time &) {}

    virtual void onRender() {}

    virtual void onImGuiRender() {}

    virtual void onEventPoll(const Event &) {}

    virtual void onEventProcess() {}

   protected:
    std::string m_name;
};
}  // namespace te

#endif /* LAYER_HPP */
