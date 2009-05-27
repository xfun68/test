#!/bin/bash

#  arglist.sh
#  测试!@和!*的不同  

E_BADARGS=65

if [ ! -n $1 ]; then
    echo "Usage:`basename $0` argument1 argument2 etc."
    exit $E_BADARGS
fi

echo 

index=1

echo "Listing args with \"\$*\""
for arg in "$*"
do
    echo "Arg #$index = $arg"
    let "index += 1"
done

index=1

echo "Listing args with \"\$@\""
for arg in "$@"
do
    echo "Arg #$index = $arg"
    let "index += 1"
done

index=1

echo "Listing args with \$*"
for arg in $*
do
    echo "Arg #$index = $arg"
    let "index += 1"
done

exit 0

