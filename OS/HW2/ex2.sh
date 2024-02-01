#!/bin/bash
# reads a number from the console and compares it to 5
echo -n Enter the value of x: 
read x

if [ $x -gt 5 ]; then
echo x is greater than 5
else
echo x is not greater than 5
fi
