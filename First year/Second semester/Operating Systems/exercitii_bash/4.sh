#!/bin/bash

if [ ! $# -eq 1 ]; then
	echo Number of arguments not ok!
	exit 1
fi

if [ ! -d $1 ]; then
	echo Not a directory!
	exit 2
fi

find "$1" -type l | while read -r link
do
	if [ ! -e "$link" ];
		echo " $link does not exist!"
	fi
done

