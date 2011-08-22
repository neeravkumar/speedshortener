#!/bin/bash
usage(){
	   echo "Usage: $0 <url-to-shorten>"
		      exit 999
}
[ $# -ne 1 ]  && usage
sed q possible
sed -i 's#//add#//add\n\t\turls["'$(sed q possible)'"] = "'$1'";#g' redirect.cpp
g++ redirect.cpp mongoose/mongoose.c -o redirect -O4 -I. -pthread -D__STDC_LIMIT_MACROS -ldl
sed -i 1d possible
