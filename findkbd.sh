#! /usr/bin/env bash

dir=/dev/input/by-path
printf "$dir/%s" $(ls $dir | grep kbd | head -n1)
