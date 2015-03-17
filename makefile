.PHONY: all clean test

archdir=bin$(shell uname -m)

all: routekeys
clean: 
	rm $(archdir)/routekeys
install:
	

routekeys: routekeys.c
	mkdir -p $(archdir)
	gcc routekeys.c -o $(archdir)/routekeys

test:
	(sleep 10; pkill routekeys)& #in case that app hangs keyboard
	kbd=$$(./findkbd.sh) ;\
	./routekeys out "$$kbd" | ./routekeys in 
