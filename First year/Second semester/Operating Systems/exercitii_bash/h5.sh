#!/bin/bash

fname=""

while [ ! -f "$fname" ]
do
	read $fname

	if [ -f "$fname" ]; then
		echo "$fname is a file! yay!"
		exit 0
	fi
done

