#include "IDACubeSearcher.h"

namespace busybin
{
  /**
   * Initialize the searcher with a PatternDatabase instance.
   * @param pPatternDatabase A pointer to a PatternDatabase that will be used
   * to get an estimated distance from a scramble to the solved state.
   */
  IDACubeSearcher::IDACubeSearcher(const PatternDatabase* pPatternDB) :
    CubeSearcher(), pPatternDB(pPatternDB)
  {
  }

  /**
   * Search the cube until goal is reached and return the moves required
   * to achieve goal.
   * @param goal The goal to achieve (isSatisfied is called on the goal).
   * @param cube The cube to search.
   * @param moveStore A MoveStore instance for retrieving moves.
   */
  vector<RubiksCube::MOVE> IDACubeSearcher::findGoal(Goal& goal,
    RubiksCube& cube, MoveStore& moveStore)
  {
    typedef RubiksCube::MOVE MOVE;
    typedef priority_queue<PrioritizedMove, vector<PrioritizedMove>,
      greater<PrioritizedMove> > moveQueue_t;

    // The IDA searcher uses pattern databases that were made using an index
    // model, so this searcher only works with an index model.
    RubiksCubeIndexModel& iCube     = static_cast<RubiksCubeIndexModel&>(cube);
    AutoTimer             timer;
    stack<Node>           nodeStack;
    Node                  curNode;
    array<uint8_t, 50>    moveInds  = {0xFF};
    bool                  solved    = false;
    uint8_t               bound     = 0;
    uint8_t               nextBound = this->pPatternDB->getNumMoves(iCube);
    moveQueue_t           successors;

    // cout << " I'm here" << endl;
    while (!solved)
    {
      if (nodeStack.empty())
      {
        if (bound != 0)
        {
          cout << "IDA*: Finished bound " << (unsigned)bound
               << ".  Elapsed time: " << timer.getElapsedSeconds() << "s."
               << endl;
        }

        // Start with the scrambled (root) node.  Depth 0, no move required.
        nodeStack.push({iCube, 0xFF, 0});

        bound     = nextBound;
        nextBound = 0xFF;
      }

      curNode = nodeStack.top();
      nodeStack.pop();

      // Keep the list of moves.  The moves end at 0xFF.
      moveInds[curNode.depth] = 0xFF;

      if (curNode.depth != 0)
        moveInds[curNode.depth - 1] = curNode.moveInd;

      for (unsigned i = 0; i < moveInds.size() && moveInds[i] != 0xFF; ++i) {
        switch (moveInds[i]) {
          case 0:
            printf("L  ");
            break;
          case 1:
            printf("L' ");
            break;
          case 2:
            printf("L2 ");
            break;
          case 3:
            printf("R  ");
            break;
          case 4:
            printf("R' ");
            break;
          case 5:
            printf("R2 ");
            break;
          case 6:
            printf("U  ");
            break;
          case 7:
            printf("U' ");
            break;
          case 8:
            printf("U2 ");
            break;
          case 9:
            printf("D  ");
            break;
          case 10:
            printf("D' ");
            break;
          case 11:
            printf("D2 ");
            break;
          case 12:
            printf("F  ");
            break;
          case 13:
            printf("F' ");
            break;
          case 14:
            printf("F2 ");
            break;
          case 15:
            printf("B  ");
            break;
          case 16:
            printf("B' ");
            break;
          case 17:
            printf("B2 ");
            break;
          default:
            printf("?  ");
        }
        // printf("%s ", moveInds[i]);
      }
      printf("\n");

      if (curNode.depth == bound)
      {
        if (goal.isSatisfied(curNode.cube))
          solved = true;
      }
      else
      {
        // This is used to sort the successors by estimated moves.
        moveQueue_t successors;

        for (uint8_t i = 0; i < 18; ++i)
        {
          if (curNode.depth == 0 || !this->pruner.prune((MOVE)i, (MOVE)curNode.moveInd))
          {
            RubiksCubeIndexModel cubeCopy(curNode.cube);

            cubeCopy.move((MOVE)i);

            uint8_t estSuccMoves = curNode.depth + 1 + this->pPatternDB->getNumMovesEx(
              cubeCopy, bound, curNode.depth + 1);

            if (estSuccMoves <= bound)
            {
              // If the twisted cube is estimated to take fewer move than the
              // current bound, push it, otherwise it's pruned.
              successors.push({cubeCopy, i, estSuccMoves});
            }
            else if (estSuccMoves < nextBound)
            {
              // The next bound is the minimum of all successor node moves that's
              // greater than the current bound.
              nextBound = estSuccMoves;
            }
          }
        }

        while (!successors.empty())
        {
          // Push the nodes in sorted order.
          nodeStack.push({
            successors.top().cube,
            successors.top().moveInd,
            (uint8_t)(curNode.depth + 1)
          });

          successors.pop();
        }
      }
    }

    // Convert the move indexes to strings.
    vector<RubiksCube::MOVE> moves;

    for (unsigned i = 0; i < moveInds.size() && moveInds[i] != 0xFF; ++i)
      moves.push_back(moveStore.getMove(moveInds[i]));

    return moves;
  }
}

