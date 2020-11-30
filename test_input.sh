#/bin/bash

make clean && make

# for D in {1..2}
for D in {1..11}
do
  # echo "./paracube -f input/t$D IDA"
  # ./paracube -f input/t$D IDA
  # ./paracube -f input/t$D IDA
  # ./paracube -f input/t$D IDA
  
  echo $D | awk '{printf "t"$0","}'
  ./paracube -f input/t$D IDA | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{printf $0","}'
  ./paracube -f input/t$D IDA | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{printf $0","}'
  ./paracube -f input/t$D IDA | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{printf $0","}'
  ./paracube -f input/t$D IDA | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{printf $0","}'
  ./paracube -f input/t$D IDA | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{print $0","}'
done
