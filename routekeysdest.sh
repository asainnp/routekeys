#!/usr/bin/env bash

keycode=$1

function isnum()    #run me in subshell
{  case "$1" in
      ''|0|*[!0-9]*) exit 1 ;; #bad num
                  *) exit 0 ;; # ok num
   esac
}

(isnum $keycode) || \
{ echo "param 1 must be valid key code (not $keycode)" >&2 ; exit 1 ; } #exitAll
case "$keycode" in
    0) exit 1 ;;
   45) exit 1 ;;                                       #key 'X' code45 - exitAll
    *) #search home RC for defined key
       #<-classic '|' pipe to while would cause subshell-exit only
       while read line; do 
          arr=(${line//:/ })   ;     [ ${#arr[@]}       -eq 3 ] || continue
                                     [ ${arr[0]} == "keycode" ] || continue
          fcode=${arr[1]}      ;                 (isnum $fcode) || continue

          [ $fcode -eq $keycode ] && { echo ${arr[2]} ; exit 0; } #exit script
       done < <(cat ~/.routekeysrc | sed -e 's/#.*$//' | tr -d '\040\011')
       exit 2  # no key found, nor exitAll requested
esac
exit 2

#--------------------------------------------------------------------------------
# routekeysdest.sh, documentation:
#--------------------------------------------------------------------------------
# reading ~/.routekeysrc for keycode-sshdestination pairs (keycode:xx:sshdest)
# return result of script should be:
#     - 0-ok, and output string to stdout is sshdestination
#     - 1-exitAll request
#     - 2-noKeyFound
#--------------------------------------------------------------------------------
