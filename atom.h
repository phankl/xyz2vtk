#ifndef atom_header
#define atom_header

#include <cmath>

struct Atom {
  public:
    int type;

    double x;
    double y;
    double z;

    Atom(int, double, double, double);
};
    
#endif
