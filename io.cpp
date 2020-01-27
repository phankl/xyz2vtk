#include "io.h"

#define MAX 1.0e300

using namespace std;

namespace io {

vector<Timestep> readXYZ(string xyzFileName)
{
  vector<Timestep> allSteps;

  ifstream file(xyzFileName);

  int atomNumber;
  double xMin = MAX;
  double xMax = -MAX;
  double yMin = MAX;
  double yMax = -MAX;
  double zMin = MAX;
  double zMax = -MAX;
  
  string line;
  vector<Atom> stepAtoms;
  bool first = true;

  // iterate over all lines in file

  while (getline(file,line)) {
    stringstream check(line);
    vector<string> words;
    string token;

    // split line into words and parse
    
    while (getline(check,token,' ')) words.push_back(token);

    if (words.size() == 4) {
      int type = stoi(words[0]);
      double x = stod(words[1]);
      double y = stod(words[2]);
      double z = stod(words[3]);
      Atom atom(type,XYZ(x,y,z));
      stepAtoms.push_back(atom);

      if (x < xMin) xMin = x;
      if (x > xMax) xMax = x;
      if (y < yMin) yMin = y;
      if (y > yMax) yMax = y;
      if (z < zMin) zMin = z;
      if (z > zMax) zMax = z;
    }
    else if (words.size() == 1) {

      // start new timestep unless first

      if (!first) {
        Timestep timestep(atomNumber,XYZ(xMin,yMin,zMin),XYZ(xMax,yMax,zMax),stepAtoms);
        allSteps.push_back(timestep);

        xMin = MAX;
        xMax = -MAX;
        yMin = MAX;
        yMax = -MAX;
        yMin = MAX;
        yMax = -MAX;
      }
      else first = false;
      
      atomNumber = stoi(words[0]);
    }
  }

  Timestep timestep(atomNumber,XYZ(xMin,yMin,zMin),XYZ(xMax,yMax,zMax),stepAtoms);
  allSteps.push_back(timestep);
  
  file.close();

  cout << "Finished reading XYZ file " << xyzFileName << "." << endl;
  cout << "Found total number of " << allSteps.size() << " time steps." << endl;

  return allSteps;
}

void writeVTK(vector<Timestep> atomData, string vtkFileName)
{
  ofstream file(vtkFileName);

  // header

  file << "# vtk DataFile Version 8.2" << endl;
  file << "Mesoscopic carbon nanotube data" << endl;
  file << "ASCII" << endl;
  file << "DATASET POLYDATA" << endl;
  file << endl;

  // data
  // first node
  

  file.close();
}

}
