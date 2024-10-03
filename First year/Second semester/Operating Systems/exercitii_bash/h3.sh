#!/bin/bash

if [ -z $# ]; then
	echo No arguments given!
	exit 1
fi

if [ ! -d $1 ]; then
	echo "$1 is not a directory!"
	exit 1
fi

total=0

find "$1" -type f -name "*.c" | while read -r file
do
	#nr_lines = $(grep -c "." "$file") ???
	nr_lines=$(grep -E -c -v ^[ \t]*$ "$file")
	echo "Nr. lines: $nr_lines"
	(($total += $nr_lines))
done

echo $total
