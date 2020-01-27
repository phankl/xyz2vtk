#include <vector>
#include <iostream>
#include <string>

#include "io.h"
#include "timestep.h"

#define SIDES 10

using namespace std;
using namespace io;

int main(int argc, char* argv[])
{
  string xyzFileName;
  string radiusString;
  string sidesString;
  double radius;
  int sides = SIDES;

  if (argc == 3) {
    xyzFileName = argv[1];
    radiusString = argv[2];
    radius = stod(radiusString);
  }
  else if (argc == 4) {
    xyzFileName = argv[1];
    radiusString = argv[2];
    sidesString = argv[3];
    radius = stod(radiusString);
    sides = stoi(sidesString);
  }
  else {
    cout << "Incorrect input format. Two input formats possible:" << endl;
    cout << "xyz2vtk XYZ_FILE_NAME CNT_RADIUS" << endl;
    cout << "xyz2vtk XYZ_FILE_NAME CNT_RADIUS POLYGON_SIDES" << endl;
    return 1;
  }

  vector<Timestep> timesteps = io::readXYZ(xyzFileName);

  return 0;
}
