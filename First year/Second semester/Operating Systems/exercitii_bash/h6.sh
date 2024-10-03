#!/bin/bash
for f in $@; do
	if test -f $f; then
		du -b $f
	fi
done | sort -n
