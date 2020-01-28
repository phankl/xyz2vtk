#include "timestep.h"

Timestep::Timestep(int stepNumberNew, int atomNumberNew,
                   XYZ xyzMinNew, XYZ xyzMaxNew, 
                   vector<Atom> atomDataNew
                   ) :
  stepNumber(stepNumberNew),
  atomNumber(atomNumberNew),
  xyzMin(xyzMinNew),
  xyzMax(xyzMaxNew),
  atomData(atomDataNew)
{
}
