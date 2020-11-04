## Rubik’s ParaCube



### SUMMARY

We are going to parallelize several algorithms and incorporate our own heuristics to find the optimal solution to any given standard 3x3x3 Rubik’s Cube case. We will perform analysis comparing these different algorithms, and attempt to generalize some for an NxNxN Rubik’s Cube. 

### Proposal
[proposal](proposal.md)

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



