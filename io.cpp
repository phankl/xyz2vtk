#include "io.h"

using namespace std;

namespace io {

vector<Timestep> readXYZ(string xyzFileName, int typeNumber)
{
  vector<Timestep> allSteps;

  ifstream file(xyzFileName);

  int stepNumber;
  int atomNumber;

  double xMin,yMin,zMin;
  double xMax,yMax,zMax;
  xMin = yMin = zMin = DBL_MAX;
  xMax = yMax = zMax = DBL_MIN;
  
  string line;
  vector<Atom> stepAtoms;
  vector<int> stepMols;
  bool first = true;

  // iterate over all lines in file

  while (getline(file,line)) {
    trim(line);
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
      double data = 0.0;

      int mol = (type / typeNumber) + 1;
      Atom atom(mol,XYZ(x,y,z),data);
      stepAtoms.push_back(atom);
      
      // check if mol is new
      vector<int>::iterator it = find(stepMols.begin(),stepMols.end(),mol);
      if (it == stepMols.end()) stepMols.push_back(mol);

      // update cell boundaries
      if (x < xMin) xMin = x;
      if (x > xMax) xMax = x;
      if (y < yMin) yMin = y;
      if (y > yMax) yMax = y;
      if (z < zMin) zMin = z;
      if (z > zMax) zMax = z;
    }
    else if (words.size() == 3) stepNumber = stoi(words[2]);
    else if (words.size() == 1) {

      // start new timestep unless first
      
      if (!first) {
        int molNumber = stepMols.size();
        Timestep timestep(stepNumber,atomNumber,molNumber,XYZ(xMin,yMin,zMin),XYZ(xMax,yMax,zMax),stepAtoms);
        allSteps.push_back(timestep);
        stepAtoms.clear();
        stepMols.clear();
        
        xMin = yMin = zMin = DBL_MAX;
        xMax = yMax = zMax = DBL_MIN;
      }
      else first = false;

      atomNumber = stoi(words[0]);
    }
  }

  int molNumber = stepMols.size();
  Timestep timestep(stepNumber,atomNumber,molNumber,XYZ(xMin,yMin,zMin),XYZ(xMax,yMax,zMax),stepAtoms);
  allSteps.push_back(timestep);
  
  file.close();

  cout << "Finished reading XYZ file " << xyzFileName << "." << endl;
  cout << "Found total number of " << allSteps.size() << " time steps." << endl;

  return allSteps;
}

vector<Timestep> readLMP(string lmpFileName)
{
  vector<Timestep> allSteps;

  ifstream file(lmpFileName);

  int stepNumber;
  int atomNumber;

  double xMin,yMin,zMin;
  double xMax,yMax,zMax;
  
  string line;
  vector<Atom> stepAtoms;
  vector<int> stepMols;
  bool first = true;
  bool step = true;
  bool number = false;
  bool box = false;
  bool xDimension = true;
  bool yDimension = false;
  bool zDimension = false;

  // iterate over all lines in file

  while (getline(file,line)) {
    trim(line);
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
      
      // check if mol is new
      vector<int>::iterator it = find(stepMols.begin(),stepMols.end(),mol);
      if (it == stepMols.end()) stepMols.push_back(mol);
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
          int molNumber = stepMols.size();
          Timestep timestep(stepNumber,atomNumber,molNumber,XYZ(xMin,yMin,zMin),XYZ(xMax,yMax,zMax),stepAtoms);
          allSteps.push_back(timestep);
          stepAtoms.clear();
          stepMols.clear();
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

  int molNumber = stepMols.size();
  Timestep timestep(stepNumber,atomNumber,molNumber,XYZ(xMin,yMin,zMin),XYZ(xMax,yMax,zMax),stepAtoms);
  allSteps.push_back(timestep);
  
  file.close();

  cout << "Finished reading LMP file " << lmpFileName << "." << endl;
  cout << "Found total number of " << allSteps.size() << " time steps." << endl;

  return allSteps;
}

void writeVTK(int polygonSides, double radius, const vector<Timestep>& timesteps, string vtkFileName)
{
  #pragma omp parallel for
  for (int i = 0; i < timesteps.size(); i++) {
    Timestep timestep = timesteps[i];
    vector<Atom> atoms = timestep.atoms;
    int stepNumber = timestep.stepNumber;
    int atomNumber = timestep.atomNumber;
    int molNumber = timestep.molNumber;
    XYZ xyzMin = timestep.xyzMin;
    XYZ xyzMax = timestep.xyzMax;
   
    string currentFileName = vtkFileName + to_string(stepNumber);
    currentFileName += ".vtk";

    ofstream file(currentFileName);
    
    // header

    file << "# vtk DataFile Version 8.2" << endl;
    file << "Mesoscopic carbon nanotube data, timestep: " << stepNumber << endl;
    file << "ASCII" << endl;
    
    // data
     
    vector<XYZ> currentPoints;
    vector<XYZ> points;
    vector<vector<int>> polygons;

    for (int j = 0; j < atomNumber; j++) {
      // first node in CNT
      if (j == 0 || atoms[j-1].mol != atoms[j].mol) {
        XYZ r1 = atoms[j].xyz;
        XYZ r2 = atoms[j+1].xyz;
        pbc(xyzMin,xyzMax,r1,r2);
        currentPoints = firstNode(polygonSides,radius,r1,r2);
      }
      // last node in CNT
      else if (j == atomNumber-1 || atoms[j].mol != atoms[j+1].mol) {
        XYZ r1 = atoms[j-1].xyz;
        XYZ r2 = atoms[j].xyz;
        if (pbc(xyzMin,xyzMax,r2,r1))
          for (int k = 0; k < polygonSides; k++)
            pbc(xyzMin,xyzMax,r2,currentPoints[k]);
        currentPoints = lastNode(r1,r2,currentPoints);
      }
      // all other nodes
      else {
        XYZ r1 = atoms[j-1].xyz;
        XYZ r2 = atoms[j].xyz;
        XYZ r3 = atoms[j+1].xyz;
        if (pbc(xyzMin,xyzMax,r2,r1))
          for (int k = 0; k < polygonSides; k++)
            pbc(xyzMin,xyzMax,r2,currentPoints[k]);
        pbc(xyzMin,xyzMax,r2,r3);
        currentPoints = centralNode(r1,r2,r3,currentPoints);
      }

      points.insert(points.end(),currentPoints.begin(),currentPoints.end());
    }

    // polygons
    
    for (int j = 0; j < atomNumber-1; j++) {
      XYZ r1 = atoms[j].xyz;
      XYZ r2 = atoms[j+1].xyz;
      int mol1 = atoms[j].mol;
      int mol2 = atoms[j+1].mol;
      if (pbc(xyzMin,xyzMax,r1,r2) || mol1 != mol2) continue;
      
      for (int k = 0; k < polygonSides; k++) {
        int index1 = j*polygonSides + k;
        int index2 = (j+1)*polygonSides + k;
        int index3 = (j+1)*polygonSides + (k+1) % polygonSides;
        int index4 = j*polygonSides + (k+1) % polygonSides;

        vector<int> polygon = {4,index1,index2,index3,index4};
        polygons.push_back(polygon);
      }
    }

    file << "DATASET POLYDATA" << endl;
    file << "POINTS " << points.size() << " FLOAT" << endl;
    for (int j = 0; j < points.size(); j++)
      file << points[j].x << " " << points[j].y << " " << points[j].z << endl;
    file << "POLYGONS " << polygons.size() << " " << 5*polygons.size() << endl;
    for (int j = 0; j < polygons.size(); j++)
      file << polygons[j][0] << " " << polygons[j][1] << " " 
           << polygons[j][2] << " " << polygons[j][3] << " "
           << polygons[j][4] << endl;
    file << "POINT_DATA " << points.size() << endl;
    file << "SCALARS MOL FLOAT 1" << endl;
    file << "LOOKUP_TABLE MOL" << endl;
    for (int j = 0; j < atomNumber; j++)
      for (int k = 0; k < polygonSides; k++)
        file << atoms[j].mol << endl;
    file << "SCALARS DATA FLOAT 1" << endl;
    file << "LOOKUP_TABLE DATA" << endl;
    for (int j = 0; j < atomNumber; j++)
      for (int k = 0; k < polygonSides; k++)
        file << atoms[j].data << endl;

    file.close();
  } 
}

vector<XYZ> firstNode(int polygonSides, double radius, const XYZ& r1, const XYZ& r2) 
{
  vector<XYZ> points(polygonSides, XYZ());
  XYZ n1 = r2 - r1;
  n1.normalise();
    
  // find vector normal to n1
  
  XYZ nBar;
  if (n1.x != 0.0)  {
    nBar.x = - (n1.y + n1.z) / n1.x;
    nBar.y = 1.0;
    nBar.z = 1.0;
  }
  else if (n1.y != 0.0) {
    nBar.x = 1.0;
    nBar.y = - (n1.x + n1.z) / n1.y;
    nBar.z = 1.0;
  }
  else {
    nBar.x = 1.0;
    nBar.y = 1.0;
    nBar.z = - (n1.x + n1.y) / n1.z;
  }
  nBar.normalise();
  nBar = radius * nBar;
    
  for (int j = 0; j < polygonSides; j++) {
    double angle = j * 2.0 * M_PI / polygonSides;
    points[j] = r1 + rotate(nBar,n1,angle);
  }

  return points;
}

vector<XYZ> centralNode(const XYZ& r1, const XYZ& r2, const XYZ& r3, const vector<XYZ>& previousPoints) 
{
  vector<XYZ> points(previousPoints.size(), XYZ());
  XYZ n1 = r2 - r1;
  XYZ n2 = r3 - r2;
  n1.normalise();
  n2.normalise();
  XYZ n12 = n1 + n2;
  n12.normalise();

  for (int i = 0; i < previousPoints.size(); i++) {
    double lambda = (n12 * (r2 - previousPoints[i])) / (n1 * n12);
    points[i] = previousPoints[i] + lambda*n1;
  }

  return points;
}

vector<XYZ> lastNode(const XYZ& r1, const XYZ& r2, const vector<XYZ>& previousPoints) 
{
  vector<XYZ> points(previousPoints.size(), XYZ());
  XYZ n = r2 - r1;
  n.normalise();

  for (int i = 0; i < previousPoints.size(); i++) {
    double lambda = n * (r1 - previousPoints[i]);
    points[i] = previousPoints[i] + lambda*n;
  }

  return points;
}

// trim from start (in place)
static inline void ltrim(string &s) {
  s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !isspace(ch);
  }));
}

// trim from end (in place)
static inline void rtrim(string &s) {
  s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !isspace(ch);
  }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(string &s) {
  ltrim(s);
  rtrim(s);
}

}
