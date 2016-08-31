#include <julia.h>
#include <math.h>

// Calculate modulus of a complex number
// (its euclidean distance from origin)
double c_modulus(double r0, double i0) {
  return sqrt((r0 * r0) + (i0 * i0));
}

void c_add(double r0, double i0, double r1, double i1, double cpx[]) {
  cpx[0] = r0 + r1;
  cpx[1] = i0 + i1;
}

void c_mult(double r0, double i0, double r1, double i1, double cpx[]) {
  cpx[0] = (r0 * r1) - (i0 * i1);
  cpx[1] = (r0 * i1) + (i0 * r1);
}

// TODO: refactor this
void norm_coords(int x, int y, 
    double r_min, 
    double x_step, double y_step, 
    double zoom_amt, 
    double x_off, double y_off, 
    double cpx[]) {
  cpx[0] = ((x * x_step) + x_off + r_min) * zoom_amt;
  cpx[1] = ((y * y_step) + y_off + r_min) * zoom_amt;
}

double calculate_r(double r0, double i0) {
  return (1 + sqrt(1 + (4 * c_modulus(r0, i0)))) / 2.0;
}
