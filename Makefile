MAKE = make -C
all: server client

server: 
	$(MAKE) ./srvr all

client:
	$(MAKE) ./clt all

clean: cleanServer cleanClient

cleanServer:
	$(MAKE) ./srvr cleanall

cleanClient:
	$(MAKE) ./clt cleanall
