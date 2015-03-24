#!/usr/bin/env bash

arch=$(uname -m)
srcbin=bin/$arch
srcscr=scripts
dstbin=/usr/bin

[ -f $srcbin/routekeys ] || { echo "routekeys for arch: $arch not compiled"; exit; }

pkill -f routekeysloop.sh
pkill routekeys

cp -v $srcbin/routekeys $dstbin
cp -v $srcscr/*.sh $dstbin

#this script is separated from makefile for possibility of installing
#on system without make tools (if target arch binary compiled and exists)
