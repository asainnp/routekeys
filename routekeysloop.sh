#!/usr/bin/env bash

kbd=$(routekeyskbd.sh)
[ $(whoami) == "root" ] && { echo "no need for sudo/root in start."; exit; }

nextdest=""

while true; do
   fname=$(mktemp -u)
   mkfifo $fname
      $nextdest sudo routekeys inp      < $fname > /dev/null &
                sudo routekeys out $kbd > $fname  
      exitcode=$?
   rm $fname

   nextdest=$(routekeysdest.sh $exitcode)
   [ $? -eq 0 ] || break
   echo "all again... nextdest($exitcode)==$nextdest"
done

echo "loop end."
