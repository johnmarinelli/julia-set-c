#ifndef LINMATH_H
#define LINMATH_H
#include <math.h>
#include <stdint.h>

#define LINMATH_H_DEFINE_VEC(n) \
typedef float vec##n[n]; \
static inline void vec##n##_add(vec##n r, vec##n const a, vec ##n const b) \
{ \
  int32_t i; \
  for (i = 0; i < n; ++i) { \
    r[i] = a[i] + b[i]; \
  } \
} \
\
static inline void vec##n##_sub(vec##n r, vec##n const a, vec ##n const b) \
{ \
  int32_t i; \
  for (i = 0; i < n; ++i) { \
    r[i] = a[i] - b[i]; \
  } \
} \
static inline void vec##n##_scale(vec##n r, vec##n const a, float const s) \
{ \
  int32_t i; \
  for (i = 0; i < n; ++i) { \
    r[i] = a[i] * s; \
  } \
} \
static inline float vec##n##_mul_inner(vec##n const a, vec ##n const b) { \
  float p = 0.0; \
  int32_t i; \
  for (i = 0; i < n; ++i) \
    p += b[i] * a[i]; \
  return p; \
} \
static inline float vec##n##_len(vec##n const v) {\
  return (float) sqrt(vec##n##_mul_inner(v,v)); \
}\
static inline void vec##n##_norm(vec##n r, vec##n const v) { \
  float k = 1.0 / vec##n##_len(v); \
  vec##n##_scale(r, v, k); \
}

LINMATH_H_DEFINE_VEC(2)
LINMATH_H_DEFINE_VEC(3)
LINMATH_H_DEFINE_VEC(4)

typedef vec4 mat4x4[4];

static inline void mat4x4_identity(mat4x4 m) {
  int32_t i, j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      m[i][j] = i == j ? 1.0 : 0.0;
    }
  }
}

static inline void mat4x4_dup(mat4x4 mtx1, mat4x4 mtx2) {
  int32_t i, j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      mtx1[i][j] = mtx2[i][j];
    }
  }
}

static inline void mat4x4_mul(mat4x4 mtx1, mat4x4 mtx2, mat4x4 mtx3) {
  mat4x4 tmp;
  int32_t k, r, c;
  for (c = 0; c < 4; ++c) {
    for (r = 0; r < 4; ++r) {
      tmp[c][r] = 0.0;
      for (k = 0; k < 4; ++k) {
        tmp[c][r] += mtx2[k][r] * mtx3[c][k];
      }
    }
  }

  mat4x4_dup(mtx1, tmp);
}

static inline void mat4x4_rotate_Z(mat4x4 mtx1, mat4x4 mtx2, float angle) {
  float s = sinf(angle);
  float c = cosf(angle);
  mat4x4 r = {
		{   c,   s, 0.f, 0.f},
		{  -s,   c, 0.f, 0.f},
		{ 0.f, 0.f, 1.f, 0.f},
		{ 0.f, 0.f, 0.f, 1.f}
  };

  mat4x4_mul(mtx1, mtx2, r);
}

static inline void mat4x4_ortho(mat4x4 M, float l, float r, float b, float t, float n, float f)
{
	M[0][0] = 2.f/(r-l);
	M[0][1] = M[0][2] = M[0][3] = 0.f;

	M[1][1] = 2.f/(t-b);
	M[1][0] = M[1][2] = M[1][3] = 0.f;

	M[2][2] = -2.f/(f-n);
	M[2][0] = M[2][1] = M[2][3] = 0.f;

	M[3][0] = -(r+l)/(r-l);
	M[3][1] = -(t+b)/(t-b);
	M[3][2] = -(f+n)/(f-n);
	M[3][3] = 1.f;
}

#endif
