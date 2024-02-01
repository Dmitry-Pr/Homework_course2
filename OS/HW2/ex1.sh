#!/bin/bash
# If section
echo "if section"
if [ "$EUID" -eq 0 ]; then # shows id of the user running the script, root has id 0 
echo "You are running the script as root"
else 
echo "You are running the script not as root"
fi
