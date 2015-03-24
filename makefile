#******************************************************************************
SHELL=/bin/bash
.PHONY: all clean test gg

archdir=bin/$(shell uname -m)
#******************************************************************************

all: $(archdir)/routekeys

clean: 
	rm $(archdir)/routekeys
install: all
	sudo ./install.sh
gg:     all
	git add . && git commit -m ... && git push ; git status
ggio:
	git pull
	make gg
#******************************************************************************
	
$(archdir):
	mkdir -p $(archdir)

$(archdir)/routekeys: $(archdir) routekeys.c
	gcc routekeys.c -o $(archdir)/routekeys -lpthread

test: install
	#kill in case that app cause keyboard hangs-up
	(echo killing in 30s; sleep 30; sudo pkill -f routekeys\*)& 
	routekeysloop.sh 
#******************************************************************************
