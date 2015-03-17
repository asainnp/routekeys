#!/usr/bin/env bash

kbd=$(routekeyskbd.sh)
[ -w $kbd ] || { echo "err: no permissions for keyboard device"; exit 1; }

nextdest=""

while true; do
   fname=$(mktemp -u)
   mkfifo $fname
      $nextdest routekeys inp      < $fname &
                routekeys out $kbd > $fname 
      exitcode=$?
   rm $fname

   nextdest=$(routekeysdest.sh $exitcode)
   [ $? -eq 0 ] && break
   echo "all again..."
done

echo "loop end."
