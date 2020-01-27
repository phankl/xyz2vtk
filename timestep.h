#ifndef timestep_header
#define timestep_header

#include <vector>

#include "atom.h"
#include "xyz.h"

using namespace std;

struct Timestep {
  public:
    int atomNumber;
    
    XYZ xyzMin;
    XYZ xyzMax;

    vector<Atom> atomData;

    Timestep(int, XYZ, XYZ, vector<Atom>);
};

#endif
