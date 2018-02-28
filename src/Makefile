#!/usr/bin/make -f

all: paxos_common acceptor paxos_lib app

paxos_common:
	$(MAKE) -C $@

acceptor:
	$(MAKE) -C $@

paxos_lib:
	$(MAKE) -C $@

app:
	$(MAKE) -C $@

clean:
	rm -rf *.o
	$(MAKE) -C paxos_common clean
	$(MAKE) -C acceptor clean
	$(MAKE) -C paxos_lib clean
	$(MAKE) -C app clean

cloc:
	cloc --exclude-dir=app,html,latex .

.PHONY: all paxos_common acceptor paxos_lib app clean cloc
