//
// Created by Tiane Zhu on 11/24/20.
//

#ifndef INC_15618_FINAL_PROJECT_CUBE_H
#define INC_15618_FINAL_PROJECT_CUBE_H

#include "debug.h"

// #define COLOR int
// #define CA 2
// #define CB 3
// #define CC 5
// #define CD 7
// #define CE 11
// #define CF 13

#define DIM 3

enum class FACE   : uint8_t {UP, LEFT, FRONT, RIGHT, BACK, DOWN};
enum class COLOR  : uint8_t {WHITE, GREEN, RED, BLUE, ORANGE, YELLOW};
enum class MOVE   : uint8_t
{
  F1, B1, L1, R1, U1, D1,
  F2, B2, L2, R2, U2, D2,
  F3, B3, L3, R3, U3, D3
  // Y, YPRIME, Y2,
  // X, XPRIME, X2,
  // Z, ZPRIME, Z2,
  // M, MPRIME, M2,
  // E, EPRIME, E2,
  // S, SPRIME, S2
};
enum class EDGE   : uint8_t {UB, UR, UF, UL, FR, FL, BL, BR, DF, DL, DB, DR};
enum class CORNER : uint8_t {ULB, URB, URF, ULF, DLF, DLB, DRB, DRF};

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
void apply(cube_t *cube, const char *operation);
void _apply(cube_t *cube, char operation);

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
cube_t *cube_cpy(cube_t *iCube);
cube_t *cube_new(bool init);
uint8_t getCornerIndex(const cube_t* cube, CORNER c);
uint8_t getCornerOrientation(const cube_t* cube, CORNER c);

#endif //INC_15618_FINAL_PROJECT_CUBE_H
