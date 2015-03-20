#******************************************************************************
SHELL=/bin/bash
.PHONY: all clean test gg

archdir=bin$(shell uname -m)
#******************************************************************************

all: $(archdir)/routekeys

clean: 
	rm $(archdir)/routekeys
install: 
	sudo ./install.sh
gg:     all
	git add . && git commit -m ... && git push ; git status
#******************************************************************************
	
$(archdir):
	mkdir -p $(archdir)

$(archdir)/routekeys: $(archdir) routekeys.c
	gcc routekeys.c -o $(archdir)/routekeys

test: install
	#kill in case that app cause keyboard hangs-up
	(sleep 20; pkill -f routekeysloop.sh; pkill routekeys)& 
	routekeysloop.sh 
#******************************************************************************
