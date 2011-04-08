#include <vector>
#include "ContactGenerator.h"
#include "Shapes.h"
#include "Island.h"

class ImpulseSolver
{
private:
public:
  static void solve(std::vector<Contact *> &scs,Real dt);
  static void applyVelocityChange(Contact *c,Vector2D *rch,Vector2D *vch);
  static void solveRelax(std::vector<Contact *> &scs,Real dt);
  static void applyVelocityChangeRelax(Contact *c,Vector2D *rch,Vector2D *vch);
  static void ImpulseSolver::solve(Island *isl,bool implode, int maxiter);
};
