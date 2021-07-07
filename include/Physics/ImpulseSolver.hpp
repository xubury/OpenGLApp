#ifndef IMPULSE_SOLVER_HPP
#define IMPULSE_SOLVER_HPP

#include "Physics/Solver.hpp"

namespace te {

class ImpulseSolver : public Solver {
   public:
    void solve(std::vector<ContactManifold> &manifolds,
               const Time &deltaTime) override;

   private:
    void initConstraints(std::vector<ContactManifold> &manifolds);

    void solveConstraint(ContactManifold &manifold, const Time &deltaTime);

    std::vector<ContactManifold> m_cachedManifolds;
};

}  // namespace te

#endif /* IMPULSE_SOLVER_HPP */
