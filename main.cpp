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
  string inputFileName;
  string vtkFileName;
  string typeNumberString;
  string radiusString;
  string sidesString;
  double radius;
  int typeNumber;
  int sides = SIDES;

  if (argc < 4 || argc > 6) {
    cout << "Incorrect input format. The following input formats are possible:" << endl;
    cout << "xyz2vtk LMP_FILE_NAME VTK_FILE_NAME CNT_RADIUS" << endl;
    cout << "xyz2vtk LMP_FILE_NAME VTK_FILE_NAME CNT_RADIUS POLYGON_SIDES" << endl;
    cout << "xyz2vtk XYZ_FILE_NAME VTK_FILE_NAME TYPE_NUMBER CNT_RADIUS" << endl;
    cout << "xyz2vtk XYZ_FILE_NAME VTK_FILE_NAME TYPE_NUMBER CNT_RADIUS POLYGON_SIDES" << endl;
    return 1;
  }

  inputFileName = argv[1];
  vtkFileName = argv[2];

  size_t xyzFound = inputFileName.find(".xyz");
  vector<Timestep> timesteps;

  if (xyzFound != string::npos) {
    typeNumberString = argv[3];
    radiusString = argv[4];
    typeNumber = stoi(typeNumberString);
    radius = stod(radiusString);
    if (argc == 6) {
      sidesString = argv[5];
      sides = stoi(sidesString);
    }

    timesteps = io::readXYZ(inputFileName, typeNumber);
  }
  else {
    radiusString = argv[3];
    radius = stod(radiusString);
    if (argc == 5) {
      sidesString = argv[4];
      sides = stoi(sidesString);
    }

    timesteps = io::readLMP(inputFileName);
  }

  io::writeVTK(sides, radius, timesteps, vtkFileName);

  return 0;
}
