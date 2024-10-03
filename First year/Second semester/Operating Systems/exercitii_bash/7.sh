#!/bin/bash

if [ ! $# -eq 1 ]; then
	echo Number of arguments not ok!
	exit 1
fi

if [ ! -f $1 ]; then
	echo " $1 is not a file!"
	exit 2
fi

#to be continued...
