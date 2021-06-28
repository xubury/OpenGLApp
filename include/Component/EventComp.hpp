#ifndef EVENT_COMP_HPP
#define EVENT_COMP_HPP

#include "ECS/ECS.hpp"
#include "Window/ActionTarget.hpp"

namespace te {

class EventComp : public Component<EventComp, EntityBase>,
                   public ActionTarget<int> {
   public:
    EventComp(const ActionMap<int> &map) : ActionTarget(map) {}
};

}  // namespace te

#endif /* EVENT_COMP_HPP */
