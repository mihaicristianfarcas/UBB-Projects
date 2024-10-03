#!/bin/bash

if [ -z $# ]; then
	echo No arguments!
	exit 1
fi

for item in $@
do
	if [ -d $item ]; then
		echo "$item is a directory!"

	elif [ -f $item ]; then
		echo "$item is a file!"

	elif echo $item | grep -E -q "^[0-9]+$"; then
		echo "$item is a number!"
	else
		echo "$item is something else!"
	fi
done

