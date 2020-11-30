//
// Created by Tiane Zhu on 11/30/20.
//

#ifndef INC_15618_FINAL_PROJECT_SOLVER_IDA_BASE_H
#define INC_15618_FINAL_PROJECT_SOLVER_IDA_BASE_H

#include "database/PatternDatabase.h"
#include "database/Korf/CornerPatternDatabase.h"
#include "solver.h"

#define FOUND 0 // TODO(tianez): must be smaller than OTHERS_FOUND
#define INFTY 0x7F

class SolverIdaBase : public Solver {

public:
  SolverIdaBase() : Solver() {
    ida_init();
  }

  ~SolverIdaBase() {
    ida_destroy();
  }

private:
  void ida_init();
  void ida_destroy();

protected:
  uint8_t h(paracube::CornerPatternDatabase *corner_db, node_t *node, int d);
  uint8_t cost(node_t *n1, node_t *n2);

  paracube::CornerPatternDatabase corner_db;
};


#endif //INC_15618_FINAL_PROJECT_SOLVER_IDA_BASE_H
