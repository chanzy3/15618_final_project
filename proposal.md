### SUMMARY

We are going to parallelize several algorithms and incorporate our own heuristics to find the optimal solution to any given standard 3x3x3 Rubik’s Cube case. We will perform analysis comparing these different algorithms, and attempt to generalize some for an NxNxN Rubik’s Cube.

### BACKGROUND

Solving the Rubik’s Cube optimally is an NP-complete problem according to Demaine, Eisenstat, Rudoy, paper. Although the specifics to the proofs is less important, by knowing the combinatorics basics, one can reason out that there are about 4.3252 x 10^19 possible states of a 3x3x3 Rubik’s cube. On average, the number of moves it takes to resolve any given Rubik’s cube is 20. The process to search for an optimal solution is therefore very memory and computationally demanding.
A very basic sequential solution that should allow the completion of such a search process assuming unlimited memory spaces is BFS. However, given the memory constraint, it may be beneficial to alleviate the memory demands by employing parallelism. Nonetheless, we expect parallelism to speed up the search process through dynamic workload balancing and appropriate communication schemes among the mostly independent, yet often prunable search branches.

### THE CHALLENGE

The workload for resolving a 3x3x3 Rubik’s cube case is a search tree with 18 degrees at each of its nodes. Given any state of the cube, there are 18 transitions. To explore the search tree, however, there is great memory demand. It has been shown that on average, the optimal solution to any given case involves 20 transitions. Therefore, the challenge existed first to prune the basic unnecessary branches.

Next, certain moves in the Rubik’s cube will return the cube to the “starting state”, or an already visited state; in such situations, a branch can be safely pruned. However, when parallelizing, such states must be effectively communicated across processes; hence, more sophistication between worker process communication is required. The price of such speedup is workload imbalance; some branches may have been traversed (or is currently being traversed by one worker process). Therefore, we need to develop a dynamic load balancing scheme. Note that, we may want to split work from a working worker process at the end of the algorithm to ease out the unexpected “long poles”. This is because our search is finding the optimal solution, similar shortest path.

A naive comparison between 18^20 ~= 1.3 * 10^25 and 4.3252 x 10^19 states shows that there will be locality of reference, i.e. certain states will be visited repeatedly when the number of moves is high. (In fact, performing any move 4 times in a row is going to result in to doing nothing, hence the ratio of reuse is very high) However, we also expect that the large number of states to give a good communication to computation ratio which incentivizes parallelization. Divergent execution will be present as explained in the previous paragraph. Hence we expect the properly communication scheme to be one of the biggest challenges that we will face.


### RESOURCES

We will not be using any starter code. We plan to spend a week building the basic version of sequential solution with a code base for showing outputs, debugging etc., We found a great resource, a paper named “Solving the Rubik’s Cube with Parallel Processing” from Aslesha Nargolkar; this paper listed a few optimization algorithms that can be used to optimize the search space, and memory bounds. Hence, we expect to refer to this paper in two stages:
  -early on in the project for the IDA algorithm and general ideas people have used in the past for optimizations;
  -later on in the project to compare our heuristics against the ones proposed in it.

### GOALS AND DELIVERABLES

#### PLAN TO ACHIEVE
For the project, we expect to complete a program that resolves the Rubik’s cube with different optimization techniques. The program should be able to resolve Rubik’s cube with different algorithms in their sequential or parallel implementation. We expect to achieve near linear speedup for different parallel implementations; we will study the speedups and understand the limiting factors and strive to improve the parallelization of at least the most optimized algorithm.

The analysis, graphs of the different algorithms will be shown at the poster session. We will also show the algorithm resolving a few cases in real time. (Note that we currently do not expect the algorithm to be able to resolve the most distant case of 24 moves within our presentation timeframe!)

#### HOPE TO ACHIEVE 
If time allows, we want to generalize an algorithm for the 3x3x3 cube into one for a general NxNxN cube. This would involve more complicated encoding schemes and more rounds of performance analysis. We may also provide insights into modelling irregular Rubik’s cube. We also accounted for the possibilities of busy weeks during the course final exam to skew our timeframe. If things are going slower than expected, we may compensate for it by not implementing a less optimized algorithm out of many that was proposed in the paper.

### PLATFORM CHOICE

We expect to build the first parallel version of the code in C++ using OpenMP or MPI. Regarding computing resources, given the state space, we expect to be using either bridges or the Xeon Phis in the Latedays clusters. It is possible that we will combine the two platforms, for a more sophisticated parallel scheme. We believe such parallel systems are suitable for the workload because of the level of independence we expect to see from the search space, and also the non trivial communication needs for optimizations.

### SCHEDULE

#### Nov 4 - Nov 16

- Implement software from scratch, Cube representations etc., and benchmarking tools for different implementations;
- Complete basic sequential implementation for resolving cubes in small number of steps;
- Parallelize the basic implementation to adapt to large number of steps;
- Measure all implemented code

#### Nov 16 - Nov 23

- Incorporate different heuristics that we came up with
- Perform measurements

#### Nov 23 - Nov 30

- Implement the more sophisticated optimization algorithms (Reid’s) proposed in the paper
- Build database for these searches
- Perform measurements

#### Nov 30 - Dec 7
- Generalize the algorithm into NxNxN cubes; this involves cube state transition encodings, and different memory bound requirements.
- Measurements and understand the new performance characteristics.

#### Dec 7 - Dec 14
- Prepare for poster session and final report.
