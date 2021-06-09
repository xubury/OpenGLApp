#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Physics/Manifold.hpp"

namespace te {

class Solver {
   public:
    virtual void solve(const std::vector<ContactManifold> &manifolds,
                       const Time &deltaTime) = 0;
};

}  // namespace te

#endif /* SOLVER_HPP */
