#!/bin/bash

dir1=$1
dir2=$2

cd $dir1
relpath=../$dir2

for file in $(dir);
do
	if [ -e $relpath/$file ] 
	then
		attr1=$(stat -c "%Y" $file)
		attr2=$(stat -c "%Y" $relpath/$file)
		if [ $attr1 -gt $attr2 ] 
		then
			cp $file $relpath/$file
		fi
		else cp $file $relpath/$file
	fi
done
cd $relpath

relpath=../$dir1

for file in $(dir);
do
	if ! [ -e $relpath/$file ] 
	then
		rm $file
	fi
done
