#!/usr/bin/env bash

keycode=$1

function isnum()
{  case "$1" in
      ''|0|*[!0-9]*) echo 1 ;; #bad num
                  *) echo 0 ;; # ok num
   esac
}
               [ $(isnum $keycode) -eq 0 ] || \
               { echo "param 1 must be valid key code (not $1)" >&2 ; exit 1 ; }

[ $keycode -eq 45 ] && exit 1               #key 'X' code45 - exit all

while read line; do
   arr=(${line//:/ })   ;     [ ${#arr[@]}       -eq 3 ] || continue
                              [ ${arr[0]} == "keycode" ] || continue
   fcode=${arr[1]}      ;     [ $(isnum $fcode)  -eq 0 ] || continue

   [ $fcode -eq $keycode ] && { echo ${arr[2]} ; exit 0; } 
done \
< <(cat ~/.routekeysrc | sed -e 's/#.*$//' | tr -d '\040\011')

exit 2  # no key found, nor exitAll requested

#--------------------------------------------------------------------------------
# routekeysdest.sh, documentation:
#--------------------------------------------------------------------------------
# reading ~/.routekeysrc for keycode-sshdestination pairs (keycode:xx:sshdest)
# return result of script should be:
#     - 0-ok, and output string to stdout is sshdestination
#     - 1-exitAll request
#     - 2-noKeyFound
#--------------------------------------------------------------------------------
