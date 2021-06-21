#ifndef CONTROLLER_LAYER_HPP
#define CONTROLLER_LAYER_HPP

#include "Core/Layer.hpp"

namespace te {

class ControllerLayer : public Layer {
   public:
    ControllerLayer();

    void onEventPoll(const Event& event) override;

    void onEventProcess() override;
};

}  // namespace te

#endif /* CONTROLLER_LAYER_HPP */
