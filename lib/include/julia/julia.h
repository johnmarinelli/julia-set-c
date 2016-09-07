#ifndef JULIA
#define JULIA

#include "types.h"

void norm_coords(uint, uint, 
    double,
    double, double,
    double,
    double, double,
    double*);
double calculate_r(double, double);
uint sq_poly_iteration(double, double, double, double, double, uint);
void complex_heat_map(uint, double, double, double, double, double*);
void julia(uint, uint,
    uint, uint,
    double, double,
    uint, uint,
    double, double, double,
    uint,
    bitmap_t*);

#endif
