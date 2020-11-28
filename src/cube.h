//
// Created by Tiane Zhu on 11/24/20.
//

#ifndef INC_15618_FINAL_PROJECT_CUBE_H
#define INC_15618_FINAL_PROJECT_CUBE_H

#include "debug.h"

#include <cstring>
#include <functional>
#include <vector>
#include <unordered_set>

#define COLOR int
#define CA 2
#define CB 3
#define CC 5
#define CD 7
#define CE 11
#define CF 13

#define DIM 3

class Cube {
public:
  // TODO(tianez): smaller size
  COLOR f[DIM][DIM];
  COLOR b[DIM][DIM];
  COLOR l[DIM][DIM];
  COLOR r[DIM][DIM];
  COLOR u[DIM][DIM];
  COLOR d[DIM][DIM];
};

inline bool operator==(const Cube& lhs, const Cube& rhs) {
  return
      memcmp(lhs.f, rhs.f, DIM * DIM * sizeof(COLOR)) == 0 &&
      memcmp(lhs.b, rhs.b, DIM * DIM * sizeof(COLOR)) == 0 &&
      memcmp(lhs.l, rhs.l, DIM * DIM * sizeof(COLOR)) == 0 &&
      memcmp(lhs.r, rhs.r, DIM * DIM * sizeof(COLOR)) == 0 &&
      memcmp(lhs.u, rhs.u, DIM * DIM * sizeof(COLOR)) == 0 &&
      memcmp(lhs.d, rhs.d, DIM * DIM * sizeof(COLOR)) == 0;
}

#define ACCUMULATE(h, ret, s) { \
  for (int i=0; i<DIM; i++) { \
    for (int j=0; j<DIM; j++) { \
      ret += ret * 31 + h(s[i][j]); \
    } \
  } \
}\

struct CubeHash {
  inline std::size_t operator()(const Cube& c) const {
    std::hash<COLOR> h;
    std::size_t ret = 0;

    ACCUMULATE(h, ret, c.f);
    ACCUMULATE(h, ret, c.b);
    ACCUMULATE(h, ret, c.l);
    ACCUMULATE(h, ret, c.r);
    ACCUMULATE(h, ret, c.u);
    ACCUMULATE(h, ret, c.d);
    return ret;
  }
};

typedef Cube cube_t;
using CubeSet = std::unordered_set<Cube, CubeHash>;

cube_t *cube_new(bool init);
cube_t *cube_cpy(cube_t *cube);

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
