#include "IDASolver.h"

IDACubeSolver::IDACubeSolver(const busybin::PatternDatabase *pPatternDB)
{
    pPatternDB = pPatternDB;
}

std::vector<MOVE> IDACubeSolver::findGoal(cube_t *cube)
{
    typedef std::priority_queue<PrioritizedMove, std::vector<PrioritizedMove>,
                                std::greater<PrioritizedMove>> moveQueue_t;

    cube_t *iCube = cube;
    bool solved = false;
    std::stack<IDACubeSolver::Node> nodeStack;
    std::array<uint8_t, 50> moveInds = {0xFF};
    uint8_t bound = 0;
    uint8_t nextBound = this->pPatternDB->getNumMoves(iCube);
    Node curNode;

    while (!solved)
    {
        if (nodeStack.empty())

        {
            if (bound != 0)
                std::cout << "IDA*: Finished bound " << (unsigned)bound << std::endl;

            nodeStack.push({iCube, 0xFF, 0});

            bound = nextBound;
            nextBound = 0xFF;
        }

        curNode = nodeStack.top();
        nodeStack.pop();

        moveInds[curNode.depth] = 0xFF;

        if (curNode.depth != 0)
            moveInds[curNode.depth - 1] = curNode.moveInd;

        if (curNode.depth == bound)
        {
            if (test_converge(curNode.iCube))
                solved = true;
        }
        else
        {
            moveQueue_t successors;

            for (int op = 0; op < TRANSITION_COUNT; op++)
            {
                // TODO: add pruner to prune trees (chengzhh)
                if (curNode.depth == 0)
                {
                    cube_t* cubeCopy = cube_cpy(iCube);
                    transition[op](cubeCopy);

                    // TODO:implement getNumMovesEx
                    uint8_t estSuccMoves = curNode.depth + 1 + this->pPatternDB->getNumMovesEx(
                        cubeCopy, bound, curNode.depth+1);
                    
                    if (estSuccMoves <= bound)
                    {
                        successors.push({cubeCopy, (uint8_t)op, estSuccMoves});
                    }
                    else if (estSuccMoves < nextBound)
                    {
                        nextBound = estSuccMoves;
                    }
                }
            }

            while(!successors.empty())
            {
                nodeStack.push({
                    &(successors.top().cube),
                    successors.top().moveInd,
                    (uint8_t)(curNode.depth+1)
                });

                successors.pop();
            }
        }
    }

    std::vector<MOVE> moves;

    for (unsigned i = 0; i < moveInds.size() && moveInds[i] != 0xFF; i++)
        moves.push_back(static_cast<MOVE>(moveInds[i]));

    return moves;
}