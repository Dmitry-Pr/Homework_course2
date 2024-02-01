#!/bin/bash

# function for sum with two parameters

add_two_num(){
    local sum=$(($1+$2))
    echo sum of $1 and $2 is $sum
}

add_two_num 2 3
