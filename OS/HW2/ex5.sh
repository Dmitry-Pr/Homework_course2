#!/bin/bash
# while section
echo "while section"
i=0
while [ $i -lt 10 ]; do # printing all digits
echo $i
((i++))
done
