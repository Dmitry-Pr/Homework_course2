#!/bin/bash
# function section
echo "function section"
greet() {
name=$1
echo "Hello, ${name:-$( whoami )}" # function with one optional parameter. The default value of the argument is the result of "whoami" comand
}

greet
greet User
