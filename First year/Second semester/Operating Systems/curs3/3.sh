#!/bin/bash

for A in $@
do
	if echo $A | grep -E -q "^[0-9]+[02468]$" && \
		( test -f $A || test -d $A ); then
			echo "Behold the amazing even file name $A!"
	fi
done

