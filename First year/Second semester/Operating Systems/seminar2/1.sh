
#!/bin/bash

# write a bash script that takes as cmd line arguments pairs consisting of
# (filename, integer) ; check if the file size is less than the specified
# integer ; ./1.sh file1 100 file2 503
# also, calculate the total size of all the files

total=0

while [ $# -gt 1 ] ; do
	echo "file $1 - int: $2"
	if [ -f $1 ]; then
		fsize=`du -b 1.sh | awk '{print $1}'`
		echo "File $1 has size ${fsize}"		
		if [ $fsize -lt $2 ]; then
			echo "File size too small."
		else
			echo "File size ok!"
		fi
		total=$(( $total + $fsize)) 	
	else
		echo "Argument $1 is not a file!"
	
	fi
	shift 2

done 

echo "Total size: $total"

