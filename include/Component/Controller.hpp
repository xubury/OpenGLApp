#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "ECS/ECS.hpp"
#include "Window/ActionTarget.hpp"

namespace te {

class Controller : public Component<Controller, EntityBase>,
                   public ActionTarget<int> {
   public:
    Controller(const ActionMap<int> &map) : ActionTarget(map) {}
};

}  // namespace te

#endif /* CONTROLLER_HPP */
