#include <vector>
#include "ContactGenerator.h"
#include "Shapes.h"
#include "Island.h"

class PenetrationSolver
{
private:
public:
  static void solve(std::vector<Contact *> &scs);
  static void solveRelax(std::vector<Contact *> &scs, Real relaxRate);
  static void solve(Island *isl, bool implode, int maxiter);
  static void applyPositionChange(Contact *c,Real *ch,Real *ah);
  static void applyPositionChangePerLevel(Contact *c,Real *ch,Real *ah, bool implode);
  static void solveRelax(std::vector<Contact *> &scs);
  static void applyPositionChangeRelax(Contact *c,Real *ch,Real *ah,Real relaxRate);
};
