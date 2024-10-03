#!/bin/bash

if [ ! $# -eq 1 ]; then
	echo Number of arguments not ok!
	exit 1
fi

if [ ! -d $1 ]; then
	echo Not a directory!
	exit 1
fi

count=0

files=$(find "$1" -type f -name "*.c")
for file in $files
do
	length=$(wc -l < "$file")

	if [ $length -gt 500 ]; then
		echo "$file"
		((count++))
	fi
	if [ $count -eq 2 ]; then
		echo Done
		break
	fi
done

