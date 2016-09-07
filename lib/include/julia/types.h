#ifndef JULIA_TYPES
#define JULIA_TYPES

#include <stdint.h>
#include <stddef.h>

typedef unsigned int uint;

/* Colored pixel */
typedef struct {
    double r, g, b;
} color_t;

/* Picture */
typedef struct {
  color_t* pixels;
  size_t width,
         height;
} bitmap_t;

#endif
