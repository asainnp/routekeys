# routekeys
route keyboard to other computer at low, tty, level

install:  
> sudo make install

run test:  
> sudo make test
> or:
> routekeys $(routekeyskbd.sh) | routekeys in

run to different comp (where routekeys is also installed):  
> routekeys $(routekeyskbd.sh) | ssh remoteCompIP routekeys in

Note:  
> all commands needs root permissions

