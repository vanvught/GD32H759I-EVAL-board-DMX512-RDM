#!/bin/bash
NPROC=2

if [ "$(uname)" == "Darwin" ]; then
     NPROC=$(sysctl -a | grep machdep.cpu.core_count | cut -d ':' -f 2)     
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
     NPROC=$(nproc)
fi

((NPROC--))

echo $NPROC

cd ..

DIR=gd32_emac_*
MAKEFILE=Makefile*.GD32

array=('DP83848')

for f in $DIR
do
	rm -rf /tmp/$f
	mkdir /tmp/$f
	for i in "${array[@]}"
	do
		mkdir /tmp/$f/$i
	done
done

for f in $DIR
do
	echo -e "\e[32m[$f]\e[0m"
	if [ -d $f ]; then
		cd "$f"	
		
		for i in "${array[@]}"
		do
		
			for m in $MAKEFILE
			do
				make -f $m -j $NPROC clean
				make -f $m -j $NPROC ENET_PHY=$i
				retVal=$?
				
				if [ $retVal -ne 0 ]; then
				 	echo "Error : " "$f" " : " "$m"
					exit $retVal
				fi
				
				SUFFIX1=$(echo $m | cut -d '-' -f 2 | cut -d '.' -f 1)
				SUFFIX2=$(echo $m | cut -d '-' -f 3 | cut -d '.' -f 1)
			
				if [ $SUFFIX1 == 'Makefile' ]
				then
					cp gd32h7xx.bin /tmp/$f/$i
				else
					echo "[" $SUFFIX1 "][" $SUFFIX2 "]"
					
					if [ -z "$SUFFIX2" ]
					then
						mkdir /tmp/$f/$i/$SUFFIX1/
						cp gd32h7xx.bin /tmp/$f/$i/$SUFFIX1
					else
						mkdir -p /tmp/$f/$i/$SUFFIX1/$SUFFIX2/
						cp gd32h7xx.bin /tmp/$f/$i/$SUFFIX1/$SUFFIX2/
					fi
				fi
							
			done
			
		done
			
		cd -
	fi
done

cd /tmp/$DIR

find . -name gd32h7xx.bin | sort | xargs ls -al
find . -name gd32h7xx.bin | xargs ls -al | wc -l

cd -
