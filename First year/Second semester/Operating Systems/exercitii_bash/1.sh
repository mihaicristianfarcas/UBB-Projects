#!/bin/bash

users=$(who | awk '{print $1}' | sort -u)

for user in "$users"
do
	nr_processes=$(ps -U "$user" | wc -l)
	echo "User: $user ; Nr. processes: $((nr_processes - 1))"
done


