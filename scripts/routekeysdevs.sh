#! /usr/bin/env bash

cd /dev/input/by-path
ls | grep 'kbd\|mouse' | grep event | xargs -L1 readlink -f

#'sort -u' used for unique list elements, more than one dev link can point
#   to some /dev/input/eventX
