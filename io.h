#ifndef io_header
#define io_header

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace io {
  vector<vector<XYZ>> readXYZ(string);
  void writeVTK(vector<vector<XYZ>>);
}

#endif
