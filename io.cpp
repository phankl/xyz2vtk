#include "io.h"

using namespace std;

namespace io {

vector<Timestep> readXYZ(string xyzFileName)
{
  vector<Timestep> allSteps;

  ifstream file(xyzFileName);

  int stepNumber;
  int atomNumber;

  double xMin,yMin,zMin;
  double xMax,yMax,zMax;
  
  string line;
  vector<Atom> stepAtoms;
  bool first = true;
  bool step = true;
  bool number = false;
  bool box = false;
  bool xDimension = true;
  bool yDimension = false;
  bool zDimension = false;

  // iterate over all lines in file

  while (getline(file,line)) {
    stringstream check(line);
    vector<string> words;
    string token;

    // split line into words and parse
    
    while (getline(check,token,' ')) words.push_back(token);

    if (words.size() == 5) {
      int mol = stoi(words[0]);
      double x = stod(words[1]);
      double y = stod(words[2]);
      double z = stod(words[3]);
      double data = stod(words[4]);
      Atom atom(mol,XYZ(x,y,z),data);
      stepAtoms.push_back(atom);
    }
    else if (words.size() == 2 && box) {
      if (xDimension) {
        xMin = stod(words[0]);
        xMax = stod(words[1]);
        xDimension = false;
        yDimension = true;
      }
      else if (yDimension) {
        yMin = stod(words[0]);
        yMax = stod(words[1]);
        yDimension = false;
        zDimension = true;
      }
      else if (zDimension) {
        zMin = stod(words[0]);
        zMax = stod(words[1]);
        zDimension = false;
        xDimension = true;
        box = false;
        step = true;
      }
    }
    else if (words.size() == 1) {
      if (step) {

        // start new timestep unless first
        
        if (!first) {
          Timestep timestep(stepNumber,atomNumber,XYZ(xMin,yMin,zMin),XYZ(xMax,yMax,zMax),stepAtoms);
          allSteps.push_back(timestep);
        }
        else first = false;

        stepNumber = stoi(words[0]);
        step = false;
        number = true;
      }
      else if (number) { 
        atomNumber = stoi(words[0]);
        number = false;
        box = true;
      }
    }
  }

  Timestep timestep(stepNumber,atomNumber,XYZ(xMin,yMin,zMin),XYZ(xMax,yMax,zMax),stepAtoms);
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
