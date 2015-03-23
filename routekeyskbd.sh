#! /usr/bin/env bash

cd /dev/input/by-path
ls | grep mouse | grep event | xargs readlink | xargs realpath | sort -u
#ls | grep 'kbd\|mouse' | grep event | xargs readlink | xargs realpath | sort -u

#'sort -u' used for unique list elements, more than one dev link can point
#   to some /dev/input/eventX
