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
gg:
	git add . && git commit -m ... && git push
#******************************************************************************
	
$(archdir):
	mkdir -p $(archdir)

$(archdir)/routekeys: $(archdir) routekeys.c
	gcc routekeys.c -o $(archdir)/routekeys

test: 
	(sleep 10; pkill routekeys)& #in case that app cause keyboard hangs-up
	routekeys out $$(routekeyskbd.sh) | routekeys in 
#******************************************************************************
