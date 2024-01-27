#!/bin/bash
# If section
echo "if section"
if [ "$EUID" -eq 0 ]; then # shows id of the user running the script, root has id 0 
echo "You are running the script as root"
else 
echo "You are running the script not as root"
fi

# while section
echo "while section"
i=0
while [ $i -lt 10 ]; do # printing all digits
echo $i
((i++))
done

# function section
echo "function section"
greet() {
name=$1
echo "Hello, ${name:-$( whoami )}" # function with one optional parameter. The default value of the argument is the result of "whoami" comand
}

greet
greet User
