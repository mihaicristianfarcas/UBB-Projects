#!/bin/bash

if [ $# -eq 0 ]; then
	echo No arguments given!
	exit 1
fi

while true
do
	for program in "$@"
	do
		if pgrep -x "$program" > dev/null; then
			echo "Dangerous program '$program'! Killing it!"
			pkill -x "$program"
		fi
	done
	sleep 1
done
	
