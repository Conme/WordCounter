#!/bin/bash
conf=$1

if [ "$conf" = "Debug" ] || [ "$conf" = "Release" ]; then
	[ -d "./build" ] && rm -r build
	mkdir build
	cd build
	cmake -DCMAKE_BUILD_TYPE=$conf ..
	make
	mkdir $conf
	mv WordCounter ./$conf
	cd $conf
else
	echo "Invalid argument for Compilation Configuration. Enter Debug or Release"
fi

