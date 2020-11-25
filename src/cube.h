//
// Created by Tiane Zhu on 11/24/20.
//

#ifndef INC_15618_FINAL_PROJECT_CUBE_H
#define INC_15618_FINAL_PROJECT_CUBE_H

#include "debug.h"

#define COLOR int
#define CA 2
#define CB 3
#define CC 5
#define CD 7
#define CE 11
#define CF 13

#define DIM 3

typedef struct cube {
  // TODO(tianez): smaller size
  COLOR f[DIM][DIM];
  COLOR b[DIM][DIM];
  COLOR l[DIM][DIM];
  COLOR r[DIM][DIM];
  COLOR u[DIM][DIM];
  COLOR d[DIM][DIM];
} cube_t;

cube_t *cube_new(bool init);

void F1(cube_t *c);
void B1(cube_t *c);
void L1(cube_t *c);
void R1(cube_t *c);
void U1(cube_t *c);
void D1(cube_t *c);

void F2(cube_t *c);
void B2(cube_t *c);
void L2(cube_t *c);
void R2(cube_t *c);
void U2(cube_t *c);
void D2(cube_t *c);

void F3(cube_t *c);
void B3(cube_t *c);
void L3(cube_t *c);
void R3(cube_t *c);
void U3(cube_t *c);
void D3(cube_t *c);

// operation must be one of F, B, L, R, U, D
void apply(cube_t *cube, char operation);

#ifdef VALID_CHECK
#define TEST_INTEGRITY(c) test_integrity(c)
#else
#define TEST_INTEGRITY(c)
#endif
// use macro above
bool test_integrity(cube_t *c);
bool test_converge(cube_t *c);

#ifdef DEBUG
#define DBG_CUBE(cube) ppp(cube)
#else
#define DBG_CUBE(cube)
#endif
// use macro above
void ppp0(COLOR s[DIM][DIM]);
void ppp(cube_t *cube);

#endif //INC_15618_FINAL_PROJECT_CUBE_H
