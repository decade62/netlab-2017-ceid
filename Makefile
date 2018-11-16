HEADERS = libf.h keyvalue.h

all: client serv1 serv2 serv3 serv4

client: client.c $(HEADERS)
	gcc client.c -o client

serv1: serv1.c $(HEADERS)
	gcc serv1.c -o serv1

serv2: serv2.c $(HEADERS)
	gcc serv2.c -o serv2

serv3: serv3.c $(HEADERS)
	gcc serv3.c -o serv3 -pthread

serv4: serv4.c $(HEADERS)
	gcc serv4.c -o serv4 -pthread
