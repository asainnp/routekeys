# routekeys
route keyboard and mouse to other computer at low, tty, level

function like: tty alternative for synergy

install on all comps:  
> git clone https://github.com/asainnp/routekeys.git  
> make install  
>  
> ... or try precompilled binary:  
> sudo ./install.sh

server config:
> cp .routekeysrc ~/.  
> edit ~/.routekeysrc and add client IPs  

run on server:
> routekeysloop.sh

Shortcuts:
> Ctrl-Alt-Shift-Esc == prefix (release after pressing all 4)  
> prefix ... X       == close all  
> prefix ... someKEY == destination defined in ~/.xroutekeysrc

Note:  
> user need to be in sudoers file,   
>      some parts of script calling sudo and need root perms.  
> automatic logon without password for server-user need to be  
>      configured on target-client comps (ssh authorized keys) 
