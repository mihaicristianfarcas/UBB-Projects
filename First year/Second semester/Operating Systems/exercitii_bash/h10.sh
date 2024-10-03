#!/bin/bash

if [ -z $# ]; then
	echo "No arguments given!"
	exit 1
fi

for file in $@
do
	if [ -f "$file" ]; then
		du "$file"
	fi
done | sort -nr

