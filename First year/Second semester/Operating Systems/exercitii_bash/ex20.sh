#!/bin/bash

# Check if number of arguments is insufficient
if [ $# -eq 0 ]; then
    echo "Insufficient number of arguments!"
    exit 1
fi

# Get permissions and directory names
permissions="$1"
directories="${@:2}"

# Loop through each directory
for directory in $directories; do
    # Check if directory exists
    if [ -d "$directory" ]; then
        echo "Directory: $directory"
        # Find all files in the directory and its subdirectories
        find "$directory" -type f | while read -r file; do
            # Get the permissions of the file
            file_permissions=$(stat -c "%a" "$file")
            # Check if permissions match
            if [ "$file_permissions" == "$permissions" ]; then
                # Create a backup file with restricted permissions
                backup_file="$file.bak"
                cp "$file" "$backup_file"
                chmod 400 "$backup_file"
                # Display information
                echo "   File: $file"
                echo "   Permissions: $file_permissions"
                echo "   Backup: $backup_file"
                echo "   Backup Permissions: $(stat -c "%a" "$backup_file")"
            fi
        done
    else
        echo "Directory: $directory"
        echo "Directory does not exist!"
    fi
done

