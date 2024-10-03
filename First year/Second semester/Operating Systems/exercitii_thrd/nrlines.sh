#!/bin/bash

files=$(find . -type f -name "*.txt")
total_lines=0
count=0

for file in $files; do
    # Use wc -l to get the line count and cut to extract only the number
    nrlines=$(wc -l < "$file")
    total_lines=$((total_lines + nrlines))
    count=$((count + 1))
done

# Ensure count is not zero to avoid division by zero
if [ $count -gt 0 ]; then
    echo $((total_lines / count))
else
    echo "No files found."
fi

