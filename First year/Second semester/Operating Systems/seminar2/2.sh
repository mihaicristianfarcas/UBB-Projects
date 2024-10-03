#!/bin/bash

# write a bash script that prints all the processes belonging to a specified user
# ./2.sh username

u=$1
# ps -ef | grep -E "^$u" same thing as down

tmp="$IFS"
IFS=$'\n'

for line in `ps -ef`; do
	user=$(echo "$line" | awk '{print $1}')
	if [ "$user" = "$u" ]; then
		echo $line
	fi
done
IFS="$tmp"

