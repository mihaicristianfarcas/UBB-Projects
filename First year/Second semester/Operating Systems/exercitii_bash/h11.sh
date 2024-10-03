#!/bin/bash

# Check if there are no command-line arguments
if [ "$#" -eq 0 ]; then
    echo "Error: No C source files provided."
    exit 1
fi

# Output file to store included libraries
output_file="included_libraries.txt"

# Loop through each command-line argument
for file in "$@"; do
    # Check if the file exists
    if [ ! -f "$file" ]; then
        echo "Error: File '$file' does not exist."
        continue
    fi

    # Use the file command to check if the file is a C source file
    file_type=$(file -b --mime-type "$file")
    if [ "$file_type" != "text/x-c" ]; then
        echo "Skipping non-C source file: $file"
        continue
    fi

    # Extract included libraries using grep
    included_libraries=$(grep -o '#include\s*<[^>]*>' "$file")

    # Print extracted libraries to output file
    echo "Included libraries in $file:" >> "$output_file"
    echo "$included_libraries" >> "$output_file"
    echo "------------------------------------" >> "$output_file"
done

echo "Included libraries extracted from C source files and saved in $output_file."

