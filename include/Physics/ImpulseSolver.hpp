#ifndef IMPULSE_SOLVER_HPP
#define IMPULSE_SOLVER_HPP

#include "Physics/Solver.hpp"

namespace te {

class ImpulseSolver : public Solver {
   public:
    void solve(const std::vector<ContactManifold> &manifolds,
               const Time &deltaTime) override;
};

}  // namespace te

#endif /* IMPULSE_SOLVER_HPP */
