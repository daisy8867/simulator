#!/bin/bash

# extract arguments to $arg
fnc=" - abc(i,j,k)"
pre=${fnc%%(*}
post=${fnc##*)}
arg=${fnc#*(}
arg=${arg%)*}

echo $fnc
echo $pre
echo $post
echo $arg
# put arguments to arr array
IFS=, read -a arr <<<"$arg"
num=${#arr[@]}

# Generate all output lines
for((i=0; i<1<<num; ++i));{
  tmp=()
  for ((j=0; j<num; ++j));{
    if ((i & (1<<j))); then tmp[j]=""
    else tmp[j]=${arr[j]}
    fi
  }
  printf -v tmp  %s "${tmp[@]}"
  echo ${tmp:1}
#  echo "$pre("${tmp:1}")$post"
}
