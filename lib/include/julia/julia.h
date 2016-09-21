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
void julia(args_t*, float*, uint64_t);

void start(uint, uint, 
    double, double,
    double,
    double, double, 
    uint);

#endif
