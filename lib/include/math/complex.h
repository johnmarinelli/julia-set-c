#ifndef COMPLEX_H
#define COMPLEX_H
// Calculate modulus of a complex number
// (its euclidean distance from origin)
inline double c_modulus(double r0, double i0) {
  return sqrt((r0 * r0) + (i0 * i0));
}

inline void c_add(double r0, double i0, double r1, double i1, double cpx[]) {
  cpx[0] = r0 + r1;
  cpx[1] = i0 + i1;
}

inline void c_mult(double r0, double i0, double r1, double i1, double cpx[]) {
  cpx[0] = (r0 * r1) - (i0 * i1);
  cpx[1] = (r0 * i1) + (i0 * r1);
}



#endif
