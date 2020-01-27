#ifndef timestep_header
#define timestep_header

#include <vector>

#include "atom.h"

using namespace std;

struct Timestep {
  public:
    int atomNumber;
    
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double zMin;
    double zMax;

    vector<Atom> atomData;

    Timestep(int, double, double, double, double, double, double, vector<Atom>);
};

#endif
