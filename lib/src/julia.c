#include <julia.h>
#include <math.h>

// Calculate modulus of a complex number
// (its euclidean distance from origin)
inline double c_modulus(double r0, double i0) {
  return sqrt((r0 * r0) + (i0 * i0));
}

