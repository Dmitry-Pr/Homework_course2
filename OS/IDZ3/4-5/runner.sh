#!/bin/bash

IP=$1
PORT=$2
NUM_CUSTOMERS=$3

./seller $IP $PORT &

for i in {1..$NUM_CUSTOMERS}
do
  ./customer $IP $PORT &
done

# run the client for output here