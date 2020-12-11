//
// Created by Tiane Zhu on 12/10/20.
//

#include "solver_ida_iter_base.h"

//////////// node_iter fn impl ////////////////////

node_iter_t *SolverIdaIterBase::node_iter_new() {
  node_iter_t *node = (node_iter_t *) malloc(sizeof(node_iter_t));
  if (node == NULL) {
    perror("malloc node");
    exit(1);
  }

  node_iter_init(node);

  return node;
}

node_iter_t *SolverIdaIterBase::node_iter_new_from_cube(cube_t* cube) {
  node_iter_t *node = (node_iter_t *) malloc(sizeof(node_iter_t));
  if (node == NULL) {
    perror("malloc node");
    exit(1);
  }
  node_iter_init_from_cube(node, cube);
  return node;
}

node_iter_t *SolverIdaIterBase::node_iter_cpy(node_iter_t *node) {
  node_iter_t *ret = node_iter_new();
  memcpy(&(ret->cube), &(node->cube), sizeof(cube_t));
  node_iter_cpy(ret, node);
  return ret;
}

void SolverIdaIterBase::node_iter_destroy(node_iter_t *node) {
  free(node);
}

void SolverIdaIterBase::node_iter_init(node_iter_t *node) {
  cube_init(&(node->cube));
  node->op = 0; // first op
  node->d = 0;
  node->g = 0;
  node->min = INFTY;
}

void SolverIdaIterBase::node_iter_init_from_cube(node_iter_t*node, cube_t *cube) {
  memcpy(&(node->cube), cube, sizeof(cube_t));
  node->op = 0; // first op
  node->d = 0;
  node->g = 0;
  node->min = INFTY;
}

void SolverIdaIterBase::node_iter_cpy(node_iter_t *dst, node_iter_t *src) {
  memcpy(&(dst->cube), &(src->cube), sizeof(cube_t));
  dst->op = src->op;
  dst->d = src->d;
  dst->g = src->g;
  dst->min = src->min;
}

uint8_t SolverIdaIterBase::h(paracube::CornerPatternDatabase *corner_db, node_iter_t *node) {
  return corner_db->getNumMoves(node->cube);
}

uint8_t SolverIdaIterBase::cost(node_iter_t *n1, node_iter_t *n2) {
  return 1;
}

