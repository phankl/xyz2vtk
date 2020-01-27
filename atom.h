#ifndef atom_header
#define atom_header

#include <cmath>

#include "xyz.h"

struct Atom {
  public:
    int type;

    XYZ xyz;

    Atom(int, XYZ);
};
    
#endif
