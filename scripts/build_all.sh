#!/bin/bash

cd ..

DIR=gd32_*

for f in $DIR
do
	echo -e "\e[32m[$f]\e[0m"
	if [ -d $f ]; then
		cd "$f"
		
		make -f Makefile.GD32 -j clean
		make -f Makefile.GD32
		retVal=$?
		if [ $retVal -ne 0 ]; then
		 	echo "Error : " "$f"
			exit $retVal
		fi

		cp gd32h7xx.bin /tmp/$f.bin
		
		cd -
	fi
done

find . -name gd32h7xx.bin | sort | xargs ls -al
find . -name gd32h7xx.bin | xargs ls -al | wc -l
