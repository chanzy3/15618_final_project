//
// Created by Tiane Zhu on 11/25/20.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

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

  c->l[0][2] = c->d[2][2];
  c->l[1][2] = c->d[2][1];
  c->l[2][2] = c->d[2][0];

  c->d[2][2] = c->r[2][0];
  c->d[2][1] = c->r[1][0];
  c->d[2][0] = c->r[0][0];

  c->r[2][0] = c->u[2][2];
  c->r[1][0] = c->u[2][1];
  c->r[0][0] = c->u[2][0];

  c->u[2][2] = arr[0];
  c->u[2][1] = arr[1];
  c->u[2][0] = arr[2];
}

void B1(cube_t *c) {
  rotate1(c->b);

  COLOR arr[3];
  arr[0] = c->r[0][2];
  arr[1] = c->r[1][2];
  arr[2] = c->r[2][2];

  c->r[0][2] = c->d[0][0];
  c->r[1][2] = c->d[0][1];
  c->r[2][2] = c->d[0][2];

  c->d[0][0] = c->l[2][0];
  c->d[0][1] = c->l[1][0];
  c->d[0][2] = c->l[0][0];

  c->l[2][0] = c->u[0][0];
  c->l[1][0] = c->u[0][1];
  c->l[0][0] = c->u[0][2];

  c->u[0][0] = arr[0];
  c->u[0][1] = arr[1];
  c->u[0][2] = arr[2];
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

  c->b[2][2] = c->d[2][2];
  c->b[1][2] = c->d[1][2];
  c->b[0][2] = c->d[0][2];

  c->d[2][2] = c->f[0][0];
  c->d[1][2] = c->f[1][0];
  c->d[0][2] = c->f[2][0];

  c->f[0][0] = arr[0];
  c->f[1][0] = arr[1];
  c->f[2][0] = arr[2];
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

  c->f[0][2] = c->d[2][0];
  c->f[1][2] = c->d[1][0];
  c->f[2][2] = c->d[0][0];

  c->d[2][0] = c->b[2][0];
  c->d[1][0] = c->b[1][0];
  c->d[0][0] = c->b[0][0];

  c->b[2][0] = arr[0];
  c->b[1][0] = arr[1];
  c->b[0][0] = arr[2];
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

  c->r[0][0] = c->u[0][0];
  c->r[0][1] = c->u[0][1];
  c->r[0][2] = c->u[0][2];

  c->u[0][0] = arr[0];
  c->u[0][1] = arr[1];
  c->u[0][2] = arr[2];
}

void D1(cube_t *c) {
  rotate1(c->d);

  COLOR arr[3];
  arr[0] = c->l[2][0];
  arr[1] = c->l[2][1];
  arr[2] = c->l[2][2];

  c->l[2][0] = c->f[2][0];
  c->l[2][1] = c->f[2][1];
  c->l[2][2] = c->f[2][2];

  c->f[2][0] = c->r[2][0];
  c->f[2][1] = c->r[2][1];
  c->f[2][2] = c->r[2][2];

  c->r[2][0] = c->u[2][0];
  c->r[2][1] = c->u[2][1];
  c->r[2][2] = c->u[2][2];

  c->u[2][0] = arr[0];
  c->u[2][1] = arr[1];
  c->u[2][2] = arr[2];
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
    fill(cube->f, CA);
    fill(cube->b, CB);
    fill(cube->l, CC);
    fill(cube->r, CD);
    fill(cube->u, CE);
    fill(cube->d, CF);
  }

  return cube;
}

void bin_color(COLOR color, int count[6]) {
  switch (color) {
    case CA:
      count[0]++;
      break;
    case CB:
      count[1]++;
      break;
    case CC:
      count[2]++;
      break;
    case CD:
      count[3]++;
      break;
    case CE:
      count[4]++;
      break;
    case CF:
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
  if (!TEST_INTEGRITY(c)) {
    return false;
  }

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
