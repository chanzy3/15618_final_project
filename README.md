## Rubik’s ParaCube



### SUMMARY

We are going to parallelize several algorithms and incorporate our own heuristics to find the optimal solution to any given standard 3x3x3 Rubik’s Cube case. We will perform analysis comparing these different algorithms, and attempt to generalize some for an NxNxN Rubik’s Cube. 

### Proposal
[proposal](proposal.md)

### Checkpoint Report
[checkpoint report](docs/cp_report/checkpoint_report.md)

### SCHEDULE

### Nov 4 - Nov 16
- [x] Implement software from scratch, Cube representations etc., and benchmarking tools for different implementations;
- [x] Complete basic sequential implementation for resolving cubes in small number of steps;
- [x] Parallelize the basic implementation to adapt to large number of steps;
- [x] Measure all implemented code

### Nov 16 - Nov 23
- [x] Implement the more sophisticated (Iterative Deepening A\* based) algorithms (Korf DB) discussed in the paper.
- [x] Construct IDA with database (Database adapted from [this repo](https://github.com/benbotto/rubiks-cube-cracker/tree/2.2.0))
- [x] Perform measurements for IDA with Korf (only corner) DB over BFS.

### Nov 23 - Nov 30
- [x] Adapt database complete. (We spend some time to port our code and make sure our model works with the given database).
- [x] Parallelize IDA with Korf DB using OMP, and perform measurements.
- [ ] Incorporate different heuristics and branch pruning techniques that we came up with (work in progress, delayed as we were blocked previous on constructing our own DB)

### Nov 30 - Dec 4
- [ ] (tianez) continued improvement of branch pruning techniques and OMP.
- [ ] (chengzhh) MPI implementation of the optimal cube solver. We expect the OMP and MPI algorithms to be different. MPI is more suitable for the more generalized NxNxN cases (which we will not implement due to the changes required for database)

### Dec 4 - Dec 7
- [ ] (team) Measurements over all implemented algorithms and understand the new performance characteristics.

### Dec 7 - Dec 9
- [ ] (team) Slack to also catch up for incomplete work from previous weeks.

### Dec 9 - Dec 14
- [ ] (team) Prepare for poster session and final report.
- [ ] (team) Paper will discuss things for future work directions as well, including, requirements for NxNxN, irregular cubes (applying constraints and heuristics) etc.,.



