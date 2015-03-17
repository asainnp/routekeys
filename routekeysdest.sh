#!/usr/bin/env bash

case "$1" in
   2) echo "ssh asain@192.168.131.35" ; exit 0 ;; #key '1' - lenovo laptop
   3) echo ""                         ; exit 0 ;; #key '2' - local comp
   4) echo "ssh    pi@192.168.134.121"; exit 0 ;; #key '3' - raspberry pi
esac
exit 1
