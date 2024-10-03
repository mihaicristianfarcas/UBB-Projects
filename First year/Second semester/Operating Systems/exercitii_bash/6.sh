#!/bin/bash

if [ ! $# -eq 1 ]; then
	echo Number of arguments not ok!
	exit 1
fi

if [ ! -d $1 ]; then
	echo Not a directory!
	exit 2
fi

find "$1" -type f -perm -u+w -perm -o+w | while read -r file
do
	 echo " File: $file"
	 echo "Permissions: $(stat -c '%A' "$file")"

	 chmod 755  "$file"

	 echo "New permissions: $(stat -c '%A' "$file")"

done
