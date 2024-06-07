#!/bin/bash

IP=$1
PORT=$2
NUM_CUSTOMERS=$3

g++ -g shop.cpp -o shop
g++ -g seller.cpp -o seller
g++ -g customer.cpp -o customer

echo "Running shop on $IP:$PORT"
./shop $IP $PORT $NUM_OUTPUTS >shopout.txt 2>&1 &
SHOP_PID=$!

echo "Running 3 sellers"
sleep 1

# Перехватываем SIGINT и передаем его серверу
trap "kill -SIGINT $SHOP_PID" SIGINT

for i in $(seq 1 3); do
  ./seller $IP $PORT >"seller$i.txt" 2>&1 &
done

sleep 1

echo "Running $NUM_CUSTOMERS customers"

for i in $(seq 1 $NUM_CUSTOMERS); do
  ./customer $IP $PORT >"customer$i.txt" 2>&1 &
  sleep 1
done

wait $SHOP_PID
