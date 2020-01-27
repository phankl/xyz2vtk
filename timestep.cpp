#include "timestep.h"

Timestep::Timestep(int atomNumberNew,
                   XYZ xyzMinNew, XYZ xyzMaxNew, 
                   vector<Atom> atomDataNew
                   ) :
  atomNumber(atomNumberNew),
  xyzMin(xyzMinNew),
  xyzMax(xyzMaxNew),
  atomData(atomDataNew)
{
}
