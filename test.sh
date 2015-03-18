#!/usr/bin/env bash

while read line; do
   echo $line
   [ -z $aaa ] && { echo 1111 ; exit 3 ; }
done < ~/.routekeysrc
echo aaaaaa

#--------------------------------------------------------------------------------
# routekeysdest.sh, documentation:
#--------------------------------------------------------------------------------
# reading ~/.routekeysrc for keycode-sshdestination pairs (keycode:xx:sshdest)
# return result of script should be:
#     - 0-ok, and output string to stdout is sshdestination
#     - 1-exitAll request
#     - 2-noKeyFound
#--------------------------------------------------------------------------------
