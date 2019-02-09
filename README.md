# routekeys
route keyboard device to other computer 
at low, tty, level

*function like:* tty alternative for synergy

install on all comps:  
> git clone https://github.com/asainnp/routekeys.git  
> make install  
>  
> ... or try precompilled binary:  sudo ./install.sh

server config:  
> cp docs/.routekeysrc ~/.  
> edit ~/.routekeysrc and add client IPs  

run on server (inside some virtual terminal or tmux/screen):  
> routekeysloop.sh  
> 
> ... or first try 'make test'

Shortcuts:  
```
LCtrl-LAlt-RAlt-RCtrl   == prefix (release after pressing all 4)  
prefix ... X            == close all (external IN->OUT kill) 
prefix ... Del          == close all (internal OUT kill) 
prefix ... someOtherKEY == if key defined in ~/.xroutekeysrc
                              then route to destComp
                              else route to localComp
```

Notes:  
* user need to be in sudoers file, on both, server and client side.  
  some parts of script calling sudo and need root perms.  
* automatic ssh logon without password for server-user need to be  
  configured on target-client comps (ssh authorized keys).  
* 'ServerAliveInterval 10' or simillar value in ~/.ssh/config on  
  server side (which is ssh client to key-clients)
* fgnotify script (github/asainnp/fgnotify) is optional dependency,
  for showing blink message on computer where keys are routed to.
* clients do not need any configuration.
* involving Xserver will probably work OK - now when mouse (evil) support is removed, everything is simple. 
  but it sometimes can be unpredictable, so in case that some xservers/driver combinations brings you to 'stucked keyboard situation', 
     - if you are stucked in client's xserver just type prefix+space (space as random undefined key) to return to main(server) comp, and 
     - if you are stucked inside main computer's xserver, just type prefix+X, which will kill routekeys loop, and return keyboard control to the system. 
  (there should NEVER be unresolvable states to think about restarting computer or accessing it with ssh from somewhere to kill routekeysloop etc.)

