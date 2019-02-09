#! /usr/bin/env bash

cd /dev/input/by-path
ls | grep 'kbd' | grep event | xargs -L1 readlink -f | sort -u

# xargs -L1   ... perform command for each line
# readlink -f ... show link target with full path
# sort -u     ... used for unique list elements, more than one dev link can point
#                 to some /dev/input/eventX

# '\|mouse' reemoved from grep 'kbd\|mouse' for using Keyboard-routing only.
# note: mouse is evil. Everything works perfect without it.
