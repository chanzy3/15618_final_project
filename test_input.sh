#/bin/bash

make clean && make

# for D in {1..2}
for D in $(seq $4 $5)
do
  # echo "./paracube -f input/t$D IDA"
  # ./paracube -f input/t$D IDA
  # ./paracube -f input/t$D IDA
  # ./paracube -f input/t$D IDA
  
  echo $D | awk '{printf "t"$0","}'
  if [ $1 == "V" ]; then
    ./paracube -f input/t$D -t 1 IDA_SEQ
    ./paracube -f input/t$D -t $3 $2
    ./paracube -f input/t$D -t $3 $2
    ./paracube -f input/t$D -t $3 $2
    ./paracube -f input/t$D -t $3 $2
    ./paracube -f input/t$D -t $3 $2
  else
    ./paracube -f input/t$D -t $3 $2 | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{printf $0","}'
    ./paracube -f input/t$D -t $3 $2 | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{printf $0","}'
    ./paracube -f input/t$D -t $3 $2 | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{printf $0","}'
    ./paracube -f input/t$D -t $3 $2 | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{printf $0","}'
    ./paracube -f input/t$D -t $3 $2 | egrep -o "Overall:.*ms" | egrep -o "[0-9]*\.[0-9]*" | awk '{print $0","}'
  fi
done
