#!/usr/bin/env bash

hstr="""
Usage:\n
\tbuild.sh\t\t\t: build all .device files \n
\tbuild.sh all\t\t\t: build all .device files \n
\tbuild.sh file\t\t\t: build specific target file \n
\tbuild.sh [file]+\t\t\t: build multiple files \n
e.g. \n
\tbuild \n
\tbuild vector_add.device \n
\tbuild vector_add.device matrix_mult.device
"""
function echohelp() {
	echo -e $hstr
}
function echogreen (){
	echo -e "\033[0;32;1m$1\033[0m"
}
function echored (){
	echo -e "\033[0;31;1m$1\033[0m"
}

function buildone() {
	file=$1
	echo "building $file.."
	if [ -e $1 ]; then
		name=${file%.*}
		echogreen "aoc -march=emulator ./$file -o ../bin/$name.aocx"
		aoc -march=emulator ./$file -o ../bin/$name.aocx
	else
		echored "file $file doesn't exist"
	fi
}

function buildall() {
	for file in `ls *.cl`; do
		buildone $file
	done
}

# no parameters or "all"
if [ x$1 == x ]; then
	set $1 all
fi

case $1 in
	all)
	       	echo "Build all targets:"
		buildall
	;;
	-h) 
		echohelp
        ;;
	--help)
	       	echohelp
	;;
	help) 
		echohelp
	;;
	*)
		for i in "$*"; do
			buildone $i
		done
	;;
esac


