# routekeys
route keyboard to other computer at low, tty, level

function like: tty alternative for synergy

install on all comps:  
> git clone https://github.com/asainnp/routekeys.git  
> make && make install  
>  
> ... if err, no make on system, try precompilled:  
> sudo ./install.sh

server config:
> cp .routekeysrc ~/.  
> edit ~/.routekeysrc add clint IPs  

run on server's emtpy tty:  
> routekeysloop  

Note:  
> user need to be in sudoers file,   
>      some parts of script calling sudo and need root perms.  
> automatic logon without password for server-user need to be  
>      configured on target-client comps (ssh authorized keys)  
