#!/usr/bin/env bash

kbd=$(routekeyskbd.sh)
[ $(whoami) == "root" ] && { echo "no need for sudo/root in start."; exit; }

nextdest=""   #route back to local comp

sudo routekeys out $kbd | \
while true; do
   #clear
   echo "routing keys to dest: $nextdest" ; [ -z "$nextdest" ] && echo local
   $nextdest sudo routekeys inp      #'routekeys inp' returns keycode in $?
   sshdest=$(routekeysdest.sh $?)
   case "$?" in                      #'routekeysdest' returns 0|1|2 in $?
      0) nextdest="ssh $sshdest" ;;  #key found
      1) break ;;                    #exitLoop requested
      2) nextdest="";                #no key found, return to local comp
   esac
done

echo "shell loop end."
