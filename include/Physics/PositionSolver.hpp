#ifndef POSITION_SOLVER_HPP
#define POSITION_SOLVER_HPP

#include "Physics/Solver.hpp"

namespace te {

class PositionSolver : public Solver {
   public:
    void solve(const std::vector<ContactManifold> &manifolds,
               const Time &deltaTime) override;
};

}  // namespace te

#endif /* POSITION_SOLVER_HPP */
