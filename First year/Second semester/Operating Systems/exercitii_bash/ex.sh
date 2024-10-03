#!/bin/bash

# Check if directory argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 directory_path"
    exit 1
fi

# Check if the directory exists
if [ ! -d "$1" ]; then
    echo "Error: '$1' is not a directory."
    exit 1
fi

# Iterate over each entry in the directory
entries=$(ls)
for entry in $entries; do
    # Get the name of the entry
    name=$(basename "$entry")

    # Check if the entry is a file
    if [ -f "$entry" ]; then
        # Get the size of the file
        size=$(du -sh "$entry" | awk '{print $1}')
        echo "File: $name, Size: $size"
    elif [ -d "$entry" ]; then
        # Count the number of items in the directory
        num_items=$(find "$entry" -maxdepth 1 -type f,d | wc -l)
        echo "Directory: $name, Number of items: $num_items"
    fi
done

