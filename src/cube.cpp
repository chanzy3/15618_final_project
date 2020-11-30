//
// Created by Tiane Zhu on 11/25/20.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cube.h"

void rotate1(COLOR s[DIM][DIM]) {
  COLOR tmp = s[0][0];
  s[0][0] = s[2][0];
  s[2][0] = s[2][2];
  s[2][2] = s[0][2];
  s[0][2] = tmp;

  tmp = s[0][1];
  s[0][1] = s[1][0];
  s[1][0] = s[2][1];
  s[2][1] = s[1][2];
  s[1][2] = tmp;
}

void F1(cube_t *c) {
  rotate1(c->f);

  COLOR arr[3];
  arr[0] = c->l[0][2];
  arr[1] = c->l[1][2];
  arr[2] = c->l[2][2];

  c->l[0][2] = c->d[0][0];
  c->l[1][2] = c->d[0][1];
  c->l[2][2] = c->d[0][2];

  c->d[0][0] = c->r[2][0];
  c->d[0][1] = c->r[1][0];
  c->d[0][2] = c->r[0][0];

  c->r[2][0] = c->u[2][2];
  c->r[1][0] = c->u[2][1];
  c->r[0][0] = c->u[2][0];

  c->u[2][2] = arr[0];
  c->u[2][1] = arr[1];
  c->u[2][0] = arr[2];

  uint8_t tmp_orientation = c->corner_orientation[ULF];
  c->corner_orientation[ULF] = c->corner_orientation[DLF];
  c->corner_orientation[DLF] = c->corner_orientation[DRF];
  c->corner_orientation[DRF] = c->corner_orientation[URF];
  c->corner_orientation[URF] = tmp_orientation;

  c->updateCornerOrientationZ(ULF);
  c->updateCornerOrientationZ(URF);
  c->updateCornerOrientationZ(DRF);
  c->updateCornerOrientationZ(DLF);
}

void B1(cube_t *c) {
  rotate1(c->b);

  COLOR arr[3];
  arr[0] = c->r[0][2];
  arr[1] = c->r[1][2];
  arr[2] = c->r[2][2];

  c->r[0][2] = c->d[2][2];
  c->r[1][2] = c->d[2][1];
  c->r[2][2] = c->d[2][0];

  c->d[2][2] = c->l[2][0];
  c->d[2][1] = c->l[1][0];
  c->d[2][0] = c->l[0][0];

  c->l[2][0] = c->u[0][0];
  c->l[1][0] = c->u[0][1];
  c->l[0][0] = c->u[0][2];

  c->u[0][0] = arr[0];
  c->u[0][1] = arr[1];
  c->u[0][2] = arr[2];

  uint8_t tmp_orientation = c->corner_orientation[ULB];
  c->corner_orientation[ULB] = c->corner_orientation[URB];
  c->corner_orientation[URB] = c->corner_orientation[DRB];
  c->corner_orientation[DRB] = c->corner_orientation[DLB];
  c->corner_orientation[DLB] = tmp_orientation;

  c->updateCornerOrientationZ(ULB);
  c->updateCornerOrientationZ(URB);
  c->updateCornerOrientationZ(DRB);
  c->updateCornerOrientationZ(DLB);
}

void L1(cube_t *c) {
  rotate1(c->l);

  COLOR arr[3];
  arr[0] = c->u[0][0];
  arr[1] = c->u[1][0];
  arr[2] = c->u[2][0];

  c->u[0][0] = c->b[2][2];
  c->u[1][0] = c->b[1][2];
  c->u[2][0] = c->b[0][2];

  c->b[2][2] = c->d[0][0];
  c->b[1][2] = c->d[1][0];
  c->b[0][2] = c->d[2][0];

  c->d[0][0] = c->f[0][0];
  c->d[1][0] = c->f[1][0];
  c->d[2][0] = c->f[2][0];

  c->f[0][0] = arr[0];
  c->f[1][0] = arr[1];
  c->f[2][0] = arr[2];

  uint8_t tmp_orientation = c->corner_orientation[DLB];
  c->corner_orientation[DLB] = c->corner_orientation[DLF];
  c->corner_orientation[DLF] = c->corner_orientation[ULF];
  c->corner_orientation[ULF] = c->corner_orientation[ULB];
  c->corner_orientation[ULB] = tmp_orientation;

  c->updateCornerOrientationX(DLB);
  c->updateCornerOrientationX(DLF);
  c->updateCornerOrientationX(ULF);
  c->updateCornerOrientationX(ULB);
}

void R1(cube_t *c) {
  rotate1(c->r);

  COLOR arr[3];
  arr[0] = c->u[0][2];
  arr[1] = c->u[1][2];
  arr[2] = c->u[2][2];

  c->u[0][2] = c->f[0][2];
  c->u[1][2] = c->f[1][2];
  c->u[2][2] = c->f[2][2];

  c->f[0][2] = c->d[0][2];
  c->f[1][2] = c->d[1][2];
  c->f[2][2] = c->d[2][2];

  c->d[0][2] = c->b[2][0];
  c->d[1][2] = c->b[1][0];
  c->d[2][2] = c->b[0][0];

  c->b[2][0] = arr[0];
  c->b[1][0] = arr[1];
  c->b[0][0] = arr[2];

  uint8_t tmp_orientation = c->corner_orientation[DRB];
  c->corner_orientation[DRB] = c->corner_orientation[URB];
  c->corner_orientation[URB] = c->corner_orientation[URF];
  c->corner_orientation[URF] = c->corner_orientation[DRF];
  c->corner_orientation[DRF] = tmp_orientation;

  c->updateCornerOrientationX(DRB);
  c->updateCornerOrientationX(DRF);
  c->updateCornerOrientationX(URF);
  c->updateCornerOrientationX(URB);
}

void U1(cube_t *c) {
  rotate1(c->u);

  COLOR arr[3];
  arr[0] = c->l[0][0];
  arr[1] = c->l[0][1];
  arr[2] = c->l[0][2];

  c->l[0][0] = c->f[0][0];
  c->l[0][1] = c->f[0][1];
  c->l[0][2] = c->f[0][2];

  c->f[0][0] = c->r[0][0];
  c->f[0][1] = c->r[0][1];
  c->f[0][2] = c->r[0][2];

  c->r[0][0] = c->b[0][0];
  c->r[0][1] = c->b[0][1];
  c->r[0][2] = c->b[0][2];

  c->b[0][0] = arr[0];
  c->b[0][1] = arr[1];
  c->b[0][2] = arr[2];

  uint8_t tmp_orientation = c->corner_orientation[ULF];
  c->corner_orientation[ULF] = c->corner_orientation[URF];
  c->corner_orientation[URF] = c->corner_orientation[URB];
  c->corner_orientation[URB] = c->corner_orientation[ULB];
  c->corner_orientation[ULB] = tmp_orientation;
}

void D1(cube_t *c) {
  rotate1(c->d);

  COLOR arr[3];
  arr[0] = c->l[2][0];
  arr[1] = c->l[2][1];
  arr[2] = c->l[2][2];

  c->l[2][0] = c->b[2][0];
  c->l[2][1] = c->b[2][1];
  c->l[2][2] = c->b[2][2];

  c->b[2][0] = c->r[2][0];
  c->b[2][1] = c->r[2][1];
  c->b[2][2] = c->r[2][2];

  c->r[2][0] = c->f[2][0];
  c->r[2][1] = c->f[2][1];
  c->r[2][2] = c->f[2][2];

  c->f[2][0] = arr[0];
  c->f[2][1] = arr[1];
  c->f[2][2] = arr[2];

  uint8_t tmp_orientation = c->corner_orientation[DLB];
  c->corner_orientation[DLB] = c->corner_orientation[DRB];
  c->corner_orientation[DRB] = c->corner_orientation[DRF];
  c->corner_orientation[DRF] = c->corner_orientation[DLF];
  c->corner_orientation[DLF] = tmp_orientation;
}

void F2(cube_t *c) { F1(c); F1(c); }
void B2(cube_t *c) { B1(c); B1(c); }
void L2(cube_t *c) { L1(c); L1(c); }
void R2(cube_t *c) { R1(c); R1(c); }
void U2(cube_t *c) { U1(c); U1(c); }
void D2(cube_t *c) { D1(c); D1(c); }

void F3(cube_t *c) { F1(c); F1(c); F1(c); }
void B3(cube_t *c) { B1(c); B1(c); B1(c); }
void L3(cube_t *c) { L1(c); L1(c); L1(c); }
void R3(cube_t *c) { R1(c); R1(c); R1(c); }
void U3(cube_t *c) { U1(c); U1(c); U1(c); }
void D3(cube_t *c) { D1(c); D1(c); D1(c); }

void apply(cube_t *cube, char operation) {
  switch(operation) {
    case 'F':
      F1(cube);
      break;
    case 'B':
      B1(cube);
      break;
    case 'L':
      L1(cube);
      break;
    case 'R':
      R1(cube);
      break;
    case 'U':
      U1(cube);
      break;
    case 'D':
      D1(cube);
      break;
    default:
      fprintf(stderr, "unknown operation '%c'\n", operation);
      exit(1);
  }
}

////////////////////////////

void fill(COLOR s[DIM][DIM], COLOR c) {
  for (int i=0; i<DIM; i++) {
    for (int j=0; j<DIM; j++) {
      s[i][j] = c;
    }
  }
}

cube_t *cube_new(bool init) {
  cube_t *cube = (cube_t *) malloc(sizeof(cube_t));
  if (cube == NULL) {
    perror("malloc cube failed");
    exit(1);
  }

  if (init) {
    fill(cube->f, CW);
    fill(cube->b, CY);
    fill(cube->l, CB);
    fill(cube->r, CG);
    fill(cube->u, CR);
    fill(cube->d, CO);
  }

  return cube;
}

cube_t *cube_cpy(cube_t *cube) {
  cube_t *c = cube_new(false);
  memcpy(c, cube, sizeof(cube_t));
  return c;
}

void bin_color(COLOR color, int count[6]) {
  switch (color) {
    case CW:
      count[0]++;
      break;
    case CY:
      count[1]++;
      break;
    case CB:
      count[2]++;
      break;
    case CG:
      count[3]++;
      break;
    case CR:
      count[4]++;
      break;
    case CO:
      count[5]++;
      break;
  }
}

bool test_integrity(cube_t *c) {
  // basic color count
  int count[6] = {0, 0, 0, 0, 0, 0};

  for (int i=0; i<DIM; i++) {
    for (int j=0; j<DIM; j++) {
      bin_color(c->f[i][j], count);
      bin_color(c->b[i][j], count);
      bin_color(c->l[i][j], count);
      bin_color(c->r[i][j], count);
      bin_color(c->u[i][j], count);
      bin_color(c->d[i][j], count);
    }
  }

  // printf("%d, %d, %d, %d, %d, %d\n", count[0], count[1], count[2], count[3], count[4], count[5]);
  for (int i=0; i<6; i++) {
    if (count[i] != DIM * DIM) {
      fprintf(stderr, "count for color C%c should be %d but is %d\n", 'A'+i, DIM * DIM, count[i]);
      return false;
    }
  }

  // TODO(tianez): check for each cubelet having a valid color combo

  return true;
}

bool same_color(COLOR s[DIM][DIM]) {
  COLOR c = s[0][0];
  for (int i=0; i<DIM; i++) {
    for (int j=0; j<DIM; j++) {
      if (s[i][j] != c) {
        return false;
      }
    }
  }
  return true;
}

bool test_converge(cube_t *c) {
#ifdef VALID_CHECK
  if (!test_integrity(c)) {
    return false;
  }
#endif

  // simplified check based on cube valid
  if (!same_color(c->f)) {
    return false;
  }
  if (!same_color(c->b)) {
    return false;
  }
  if (!same_color(c->l)) {
    return false;
  }
  if (!same_color(c->r)) {
    return false;
  }
  if (!same_color(c->u)) {
    return false;
  }
  if (!same_color(c->d)) {
    return false;
  }

  return true;
}

// TODO(tianez): remove
void ppp0(COLOR s[DIM][DIM]) {
  for (int i=0; i<DIM; i++) {
    for (int j=0; j<DIM; j++) {
      printf("%2d ", s[i][j]);
    }
    printf("\n");
  }
}

void ppp(cube_t *cube) {
  printf("F%9.9s| B%9.9s| L%9.9s| R%9.9s| U%9.9s| D\n", "", "", "", "", "");
  for (int i=0; i<DIM; i++) {
    for (int j=0; j<DIM; j++) {
      printf("%2d ", cube->f[i][j]);
    }
    printf(" | ");
    for (int j=0; j<DIM; j++) {
      printf("%2d ", cube->b[i][j]);
    }
    printf(" | ");
    for (int j=0; j<DIM; j++) {
      printf("%2d ", cube->l[i][j]);
    }
    printf(" | ");
    for (int j=0; j<DIM; j++) {
      printf("%2d ", cube->r[i][j]);
    }
    printf(" | ");
    for (int j=0; j<DIM; j++) {
      printf("%2d ", cube->u[i][j]);
    }
    printf(" | ");
    for (int j=0; j<DIM; j++) {
      printf("%2d ", cube->d[i][j]);
    }
    printf("\n");
  }
  /*
  printf("F\n");
  ppp0(cube->f);
  printf("B\n");
  ppp0(cube->b);
  printf("L\n");
  ppp0(cube->l);
  printf("R\n");
  ppp0(cube->r);
  printf("U\n");
  ppp0(cube->u);
  printf("D\n");
  ppp0(cube->d);
   */
}

inline void Cube::getCorner(CORNER ind, COLOR &a, COLOR &b, COLOR &c) const {
  switch (ind) {
    case ULB:
      a = this->u[0][0];
      b = this->l[0][0];
      c = this->b[0][2];
      break;
    case URB:
      a = this->u[0][2];
      b = this->r[0][2];
      c = this->b[0][0];
      break;
    case URF:
      a = this->u[2][2];
      b = this->r[0][0];
      c = this->f[0][2];
      break;
    case ULF:
      a = this->u[2][0];
      b = this->l[0][2];
      c = this->f[0][0];
      break;
    case DLF:
      a = this->d[0][0];
      b = this->l[2][2];
      c = this->f[2][0];
      break;
    case DLB:
      a = this->d[2][0];
      b = this->l[2][0];
      c = this->b[2][2];
      break;
    case DRB:
      a = this->d[2][2];
      b = this->r[2][2];
      c = this->b[2][0];
      break;
    case DRF:
      a = this->d[0][2];
      b = this->r[2][0];
      c = this->f[2][2];
      break;
    default:
      a = -1;
      b = -1;
      c = -1;
      break;
  }
}

char ts(int c) {
  switch (c) {
    case CR:
      return 'R';
    case CB:
      return 'B';
    case CY:
      return 'Y';
    case CW:
      return 'W';
    case CO:
      return 'O';
    case CG:
      return 'G';
  }
  return '?';
}

uint8_t Cube::getCornerIndex(CORNER ind) const {
  COLOR a, b, c;
  getCorner(ind, a, b, c);
  // printf("!ZZZ! %d, %c%c%c\n", ind, ts(a), ts(b), ts(c));
  switch (a * b * c) {
    case CR * CB * CY:
      return 0;
    case CR * CG * CY:
      return 1;
    case CR * CG * CW:
      return 2;
    case CR * CB * CW:
      return 3;
    case CO * CB * CW:
      return 4;
    case CO * CB * CY:
      return 5;
    case CO * CG * CY:
      return 6;
    case CO * CG * CW:
      return 7;
    default:
      // TODO(tianez): error
      return -1;
  }
}

uint8_t Cube::getCornerOrientation(CORNER ind) const {
  return this->corner_orientation[ind];
}

/**
   * Helper to update the orientation of corners on an L or R turn.
   */
inline void Cube::updateCornerOrientationX(CORNER ind)
{
  // The new orientation differs based on its distance from home.  E.g.
  // moving corner 0 (RBY) left moves it from ULB (0) to ULF (3).  That's an
  // odd distance (0+3), so the orientation is 1 (Y on top).  Moving corner 0
  // (RBY) left prime from ULF (3) to ULB (0) is even (0+0), so the
  // orientation is 2.
  bool evenDist = ((uint8_t)ind + this->getCornerIndex(ind)) % 2 == 0;

  if (this->getCornerOrientation(ind) == 0)
    this->corner_orientation[ind] = evenDist ? 2 : 1;
  else if (this->getCornerOrientation(ind) == 1)
    this->corner_orientation[ind] = evenDist ? 0 : 2;
  else if (this->getCornerOrientation(ind) == 2)
    this->corner_orientation[ind] = evenDist ? 1 : 0;
}

/**
   * Helper to update the orientation of corners on an F or B turn.
   */
inline void Cube::updateCornerOrientationZ(CORNER ind)
{
  // Moving corner 3 (RBW) front moves it from ULF (3) to URF (2).  That's an
  // odd distance (3+2), so the orientation is 2 (B on top).  Moving corner 3
  // (RBW) front prime from URF (2) to ULF (3) is even (3+3), so the
  // orientation is 1.
  bool evenDist = ((uint8_t)ind + this->getCornerIndex(ind)) % 2 == 0;

  if (this->getCornerOrientation(ind) == 0)
    this->corner_orientation[ind] = evenDist ? 1 : 2;
  else if (this->getCornerOrientation(ind) == 1)
    this->corner_orientation[ind] = evenDist ? 2 : 0;
  else if (this->getCornerOrientation(ind) == 2)
    this->corner_orientation[ind] = evenDist ? 0 : 1;
}


