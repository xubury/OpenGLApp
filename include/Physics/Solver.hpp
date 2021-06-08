#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Physics/Collider.hpp"

namespace te {

class Solver {
   public:
    virtual void solve(const std::vector<ContactManifold> &manifolds,
                       const Time &deltaTime) = 0;
};

}  // namespace te

#endif /* SOLVER_HPP */
