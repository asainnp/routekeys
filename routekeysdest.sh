#!/usr/bin/env bash

case "$1" in
   1002) echo "ssh    192.168.131.35" ; exit 0 ;; #key '1' - lenovo laptop
   1003) echo ""                      ; exit 0 ;; #key '2' - local comp
   1004) echo "ssh pi@192.168.134.121"; exit 0 ;; #key '3' - raspberry pi
esac
exit 1
