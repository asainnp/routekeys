#!/usr/bin/env bash

function isnum()    #run me in subshell
{  case "$1" in
      ''|0|*[!0-9]*) exit 1 ;; #bad num
                  *) exit 0 ;; # ok num
   esac
}

if [ "$1" == "listdestinations" ]; then
   # show RC file       remove comments     remove spaces/tabs
   cat ~/.routekeysrc | sed -e 's/#.*$//' | tr -d '\040\011' | \
   while read line; do 
      # find lines with format " keycode : XX,YY... : sshdestination "
      arr=(${line//:/ })   ;     [ ${#arr[@]}       -eq 3 ] || continue
                                 [ ${arr[0]} == "keycode" ] || continue
      fcodes=${arr[1]}     ;                # (isnum $fcode) || continue
      if [ "$2" == "withcodes" ]; then echo $fcodes ${arr[2]} #simple space
      else                             echo ${arr[2]} ;   fi
   done 
   exit 0
fi

keycode=$1
(isnum $keycode) || \
{ echo "param 1 must be valid key code (not $keycode)" >&2 ; exit 1 ; } #exitAll
case "$keycode" in
     0) exit 1 ;;
45|111) exit 1 ;;                         #key 'X' or 'del' code45/111 - exitAll
     *) #search home RC for defined key
        #<-classic '|' pipe to while would cause subshell-exit only
        while read line; do 
           arr=($line) ; scodes=${arr[0]}        ; adest=${arr[1]}
                         acodes=( ${scodes//,/ } )
           for i in ${acodes[@]}; do 
              (isnum $i) &&[ "$i" -eq "$keycode" ] && \
              { echo $adest; exit 0; } #exit whole script!!!
           done 
        done < <($0 listdestinations withcodes)
        exit 2  ;; # no key found, nor exitAll requested
esac
exit 2

#--------------------------------------------------------------------------------
# routekeysdest.sh, documentation:
#--------------------------------------------------------------------------------
# reading ~/.routekeysrc for keycode-sshdestination pairs (keycode:xx,yy:sshdest)
#
# routekeysdest.sh listdestinations ... just list destinations and exit 0
#                                       (used in makefile with such param)
# routekeysdest.sh listdestinations withcodes ... -||-,      same +codes
# routekeysdest.sh XX - show destination for code XX (or exit 1/2)
# return result of script should be:
#     - 0-ok, and output string to stdout is sshdestination
#     - 1-exitAll request
#     - 2-noKeyFound
#--------------------------------------------------------------------------------
