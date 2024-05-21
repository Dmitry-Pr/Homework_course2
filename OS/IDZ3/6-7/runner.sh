#!/bin/bash

IP=$1
PORT=$2
NUM_CUSTOMERS=$3

./shop $IP $PORT > /dev/null 2>&1 &
SHOP_PID=$!
sleep 2

# Перехватываем SIGINT и передаем его серверу
trap "kill -SIGINT $SHOP_PID" SIGINT

for i in $(seq 1 3)
do
  ./seller $IP $PORT > /dev/null 2>&1 &
done

sleep 2

# Вывод этой программы будет показан
./output $IP $PORT &

sleep 2

echo "Running $NUM_CUSTOMERS customers"

for i in $(seq 1 $NUM_CUSTOMERS)
do
  ./customer $IP $PORT > /dev/null 2>&1 &
  sleep 1
done

wait $SHOP_PID