#include "timestep.h"

Timestep::Timestep(int atomNumberNew,
                   double xMinNew, double xMaxNew, 
                   double yMinNew, double yMaxNew,
                   double zMinNew, double zMaxNew,
                   vector<Atom> atomDataNew
                   ) :
  atomNumber(atomNumberNew),
  xMin(xMinNew),
  xMax(xMaxNew),
  yMin(yMinNew),
  yMax(yMaxNew),
  zMin(zMinNew),
  zMax(zMaxNew),
  atomData(atomDataNew)
{
}
