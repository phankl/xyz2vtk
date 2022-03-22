#ifndef io_header
#define io_header

#include <algorithm>
#include <cctype>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <fstream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "atom.h"
#include "timestep.h"
#include "xyz.h"

using namespace std;

namespace io {
  vector<Timestep> readXYZ(string, int);
  vector<Timestep> readLMP(string);
  void writeVTK(int, double, const vector<Timestep>&, string);

  vector<XYZ> firstNode(int, double, const XYZ&, const XYZ&);
  vector<XYZ> centralNode(const XYZ&, const XYZ&, const XYZ&, const vector<XYZ>&);
  vector<XYZ> lastNode(const XYZ&, const XYZ&, const vector<XYZ>&);

  static inline void ltrim(string&);
  static inline void rtrim(string&);
  static inline void trim(string&);
}

#endif
