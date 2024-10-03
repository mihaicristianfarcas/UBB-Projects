#!/bin/bash

# Check if the folder path is provided as an argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <folder>"
    exit 1
fi

# Get the folder path from the command line argument
folder=$1

# Check if the provided path is a directory
if [ ! -d "$folder" ]; then
    echo "Error: $folder is not a directory."
    exit 1
fi

# Store initial state of the folder
initial_state=$(ls -lR "$folder")

# Infinite loop to continuously monitor the folder
while true; do
    # Get current state of the folder
    current_state=$(ls -lR "$folder")
    
    # Compare current state with initial state
    if [ "$initial_state" != "$current_state" ]; then
        echo "Something has changed in $folder"
        
        # Update initial state to current state
        initial_state="$current_state"
    fi

    # Sleep for 1 second before checking again
    sleep 1
done

