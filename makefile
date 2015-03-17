#******************************************************************************
.PHONY: all clean test

archdir=bin$(shell uname -m)
#******************************************************************************

all: $(archdir)/routekeys

clean: 
	rm $(archdir)/routekeys
install: 
	cp $(archdir)/routekeys /usr/bin/.
	cp routekeyskbd.sh /usr/bin/.
#******************************************************************************
	
$(archdir):
	mkdir -p $(archdir)

$(archdir)/routekeys: $(archdir) routekeys.c
	gcc routekeys.c -o $(archdir)/routekeys

test: 
	(sleep 10; pkill routekeys)& #in case that app cause keyboard hangs-up
	routekeys out $$(routekeyskbd.sh) | routekeys in 
#******************************************************************************
