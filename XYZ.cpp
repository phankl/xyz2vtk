#include "xyz.h"

XYZ::XYZ(double xNew, double yNew, double zNew) :
  x(xNew),
  y(yNew),
  z(zNew)
{
}

void XYZ::normalise()
{
  double length = sqrt(x*x + y*y + z*z);
  x /= length;
  y /= length;
  z /= length;
}

XYZ operator + (const XYZ& a, const XYZ& b) {
  return XYZ(a.x + b.x,a.y + b.y,a.z + b.z);
}

XYZ operator - (const XYZ& a, const XYZ& b) {
  return XYZ(a.x - b.x,a.y - b.y,a.z - b.z);
}

XYZ operator * (double a, const XYZ& b) {
  return XYZ(a*b.x,a*b.y,a*b.z);
}

double operator * (const XYZ& a, const XYZ& b) {
  return a.x*b.x + a.y*b.y + a.z*b.z;
}
