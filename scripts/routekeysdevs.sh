#! /usr/bin/env bash

cd /dev/input/by-path
ls | grep 'kbd\|mouse' | grep event | xargs -L1 readlink -f | sort -u

# xargs -L1   ... perform command for each line
# readlink -f ... show link target with full path
# sort -u     ... used for unique list elements, more than one dev link can point
#                 to some /dev/input/eventX

# '\|mouse' can be removed from grep for using Keyboard-routing only
