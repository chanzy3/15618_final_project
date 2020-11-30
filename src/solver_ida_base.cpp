//
// Created by Tiane Zhu on 11/30/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "solver_ida_base.h"

char corner_db_input_filename[19] = "data/corner.pdb";

void SolverIdaBase::ida_init() {
  if (!corner_db.fromFile(corner_db_input_filename)) {
    printf("failed to init corner_db from file '%s'\n", corner_db_input_filename);
    exit(1);
  }
}

void SolverIdaBase::ida_destroy() {
  // delete this->corner_db;
}

uint8_t SolverIdaBase::h(paracube::CornerPatternDatabase *corner_db, node_t *node, int d) {
  //
  // return 1;
  DBG_CUBE(node->cube);
  return corner_db->getNumMoves(*node->cube);
}

uint8_t SolverIdaBase::cost(node_t *n1, node_t *n2) {
  return 1;
}
