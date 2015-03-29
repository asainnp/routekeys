#******************************************************************************
SHELL=/bin/bash
.PHONY: clean test gg

archdir=bin/$(shell uname -m)
#******************************************************************************

all: $(archdir)/routekeys
clean: 
	rm $(archdir)/routekeys
install: $(archdir)/routekeys $(wildcard scripts/*.sh)
	sudo ./install && touch install

gg:     install
	git add . && git commit -m ... ; : 
	git push
ggio:
	printf "%.0s-" {1..30} && cat /etc/hostname
	git pull && make gg

ggallcomps: gg
	scripts/routekeysdest.sh listdestinations |         \
	while read line; do                                 \
	   (< /dev/null ssh $$line 'cd github/routekeys; make ggio'); done
	git pull
#******************************************************************************
	
$(archdir):
	mkdir -p $(archdir)

$(archdir)/routekeys: $(archdir) routekeys.c
	gcc routekeys.c -o $(archdir)/routekeys -lpthread

test: install
	#kill in case that app cause keyboard hangs-up
	(echo killing in 30s; sleep 30; sudo ./install justkill)& 
	routekeysloop.sh 
#******************************************************************************
