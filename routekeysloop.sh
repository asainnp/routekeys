#!/usr/bin/env bash

kbd=$(routekeyskbd.sh)
[ $(whoami) == "root" ] && { echo "no need for sudo/root in start."; exit; }

nextdest=""   #route back to local comp

while true; do
   sudo routekeys out $kbd | $nextdest sudo routekeys inp
   exitkeycode=${PIPESTATUS[0]}

   sshdest=$(routekeysdest.sh $exitkeycode)
   case "$?" in
      0) nextdest="ssh $sshdest" ;;  #key found
      1) break ;;                    #exitLoop requested
      2) nextdest="";                #no key found, return to local comp
   esac
   echo "all again... nextdest($exitcode)==$nextdest"
done

echo "loop end."
