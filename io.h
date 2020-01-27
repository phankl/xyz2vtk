#ifndef io_header
#define io_header

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "atom.h"
#include "timestep.h"

using namespace std;

namespace io {
  vector<Timestep> readXYZ(string);
  void writeVTK(vector<Timestep>, string);
}

#endif
