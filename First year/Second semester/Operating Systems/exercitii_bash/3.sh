#!/bin/bash

if [ ! $# -eq 1 ]; then
	echo Number of arguments not ok!
	exit 1
fi

if [ ! -d $1 ]; then
	echo Not a directory!
	exit 2
fi

files=$(find "$1" -type f -name "*.log")
for file in $files
do
	sort -o "$file" "$file"	
done

