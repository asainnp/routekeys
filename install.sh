#!/usr/bin/env bash

arch=$(uname -m)
srcbin=bin$arch
dstbin=/usr/bin

[ -f $srcbin/routekeys ] || { echo "routekeys for arch: $arch not compiled"; exit; }

cp -v $srcbin/routekeys $dstbin
cp -v routekeys{kbd,dest,loop}.sh $dstbin

#this script is separated from makefile for possibility of installing
#on system without make tools (if target arch binary exists)
