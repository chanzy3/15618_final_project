#!/bin/sh

# mpirun -np 2 ./paracube IDA_MPI2 -p 2 -f input/t13_all_i1
# mpirun -np 3 ./paracube IDA_MPI2 -p 3 -f input/t13_all_i1
# mpirun -np 5 ./paracube IDA_MPI2 -p 5 -f input/t13_all_i1
# mpirun -np 5 ./paracube IDA_MPI2 -p 9 -f input/t13_all_i1
# mpirun -np 17 ./paracube IDA_MPI2 -p 17 -f input/t13_all_i1
# mpirun -np 33 ./paracube IDA_MPI2 -p 33 -f input/t13_all_i1
# mpirun -np 65 ./paracube IDA_MPI2 -p 65 -f input/t13_all_i1
# mpirun -np 129 ./paracube IDA_MPI2 -p 129 -f input/t13_all_i1

#     mpirun -np 2 ./paracube IDA_MPI2 -p 2 -f input/t12
#     mpirun -np 3 ./paracube IDA_MPI2 -p 3 -f input/t12  
#     mpirun -np 5 ./paracube IDA_MPI2 -p 5 -f input/t12
#     mpirun -np 5 ./paracube IDA_MPI2 -p 9 -f input/t12
#   mpirun -np 17 ./paracube IDA_MPI2 -p 17 -f input/t12
#   mpirun -np 33 ./paracube IDA_MPI2 -p 33 -f input/t12
#   mpirun -np 65 ./paracube IDA_MPI2 -p 65 -f input/t12
# mpirun -np 129 ./paracube IDA_MPI2 -p 129 -f input/t12

#     mpirun -np 2 ./paracube IDA_MPI2 -p 2 -f input/t11
#     mpirun -np 3 ./paracube IDA_MPI2 -p 3 -f input/t11  
#     mpirun -np 5 ./paracube IDA_MPI2 -p 5 -f input/t11
#     mpirun -np 5 ./paracube IDA_MPI2 -p 9 -f input/t11
#   mpirun -np 17 ./paracube IDA_MPI2 -p 17 -f input/t11
#   mpirun -np 33 ./paracube IDA_MPI2 -p 33 -f input/t11
#   mpirun -np 65 ./paracube IDA_MPI2 -p 65 -f input/t11
# mpirun -np 129 ./paracube IDA_MPI2 -p 129 -f input/t11

#     mpirun -np 2 ./paracube IDA_MPI2 -p 2 -f input/t10
#     mpirun -np 3 ./paracube IDA_MPI2 -p 3 -f input/t10  
#     mpirun -np 5 ./paracube IDA_MPI2 -p 5 -f input/t10
#     mpirun -np 5 ./paracube IDA_MPI2 -p 9 -f input/t10
#   mpirun -np 17 ./paracube IDA_MPI2 -p 17 -f input/t10
#   mpirun -np 33 ./paracube IDA_MPI2 -p 33 -f input/t10
#   mpirun -np 65 ./paracube IDA_MPI2 -p 65 -f input/t10
# mpirun -np 129 ./paracube IDA_MPI2 -p 129 -f input/t10

mpirun -np 3 ./paracube IDA_MPI4 -p 2 -f input/t13_all_i1
mpirun -np 5 ./paracube IDA_MPI4 -p 5 -f input/t13
# mpirun -np 2 ./paracube IDA_MPI4 -p 2 -f input/t13
    # mpirun -np 2 ./paracube IDA_MPI2 -p 2 -f input/t13
    mpirun -np 3 ./paracube IDA_MPI2 -p 3 -f input/t13  
    mpirun -np 5 ./paracube IDA_MPI2 -p 5 -f input/t13
    mpirun -np 9 ./paracube IDA_MPI2 -p 9 -f input/t13
  mpirun -np 17 ./paracube IDA_MPI2 -p 17 -f input/t13
  mpirun -np 33 ./paracube IDA_MPI2 -p 33 -f input/t13
  mpirun -np 65 ./paracube IDA_MPI2 -p 65 -f input/t13
mpirun -np 129 ./paracube IDA_MPI2 -p 129 -f input/t13

    mpirun -np 2 ./paracube IDA_MPI2 -p 2 -f input/t13_scale
    mpirun -np 3 ./paracube IDA_MPI2 -p 3 -f input/t13_scale  
    mpirun -np 5 ./paracube IDA_MPI2 -p 5 -f input/t13_scale
    mpirun -np 9 ./paracube IDA_MPI2 -p 9 -f input/t13_scale
  mpirun -np 17 ./paracube IDA_MPI2 -p 17 -f input/t13_scale
  mpirun -np 33 ./paracube IDA_MPI2 -p 33 -f input/t13_scale
  mpirun -np 65 ./paracube IDA_MPI2 -p 65 -f input/t13_scale
mpirun -np 129 ./paracube IDA_MPI2 -p 129 -f input/t13_scale



# ./paracube IDA -p 2 -f input/t13_last_i1
# mpirun -np 2 ./paracube IDA_MPI4 -p 2 -f input/t13_last_i1
# mpirun -np 3 ./paracube IDA_MPI4 -p 3 -f input/t13_last_i1  
# mpirun -np 5 ./paracube IDA_MPI4 -p 5 -f input/t13_last_i1
# mpirun -np 9 ./paracube IDA_MPI4 -p 9 -f input/t13_last_i1
# mpirun -np 17 ./paracube IDA_MPI4 -p 17 -f input/t13_last_i1
# mpirun -np 33 ./paracube IDA_MPI4 -p 33 -f input/t13_last_i1
# mpirun -np 65 ./paracube IDA_MPI4 -p 65 -f input/t13_last_i1
# mpirun -np 129 ./paracube IDA_MPI4 -p 129 -f input/t13_last_i1