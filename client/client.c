#include "libf.h"

int main(int argc, char **argv) {
	char function, msg[2], response[1025], request[1025], *ch;		//TODO: remove response, use request instead or change name to 'buffer'
	struct addrinfo hints, *servinfo;
	int sockFd, iAddrinfo, tmp;
	register short argCnt = 3;										//argument 3 will be the first command's function

	if (argc < 5) errArgs();
	//initialization for getaddrinfo
	memset(&hints, 0, sizeof(hints));								//zeroing the flags
	hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	iAddrinfo = getaddrinfo(argv[1], argv[2], &hints, &servinfo);	//actual lookup takes place here
	//if (iAddrinfo != 0) { errAddrInfo(); return 0; }				//TBR??
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	tmp = connect(sockFd, servinfo->ai_addr, servinfo->ai_addrlen);	//tmp holds the result of connect
	//if (tmp == 0) printf("Socket connected!\n");					//TBR
	//else printf("Socket connection unsuccessful!\n");				//TBR

	while (argCnt + 1 < argc) {										//commands' loop
		if (strcmp(argv[argCnt], "get") == 0) {						//if command is 'get'
			function = 'g';
			ch = request;											//ch is a temporary char pointer to handle our request before we send it
			*(ch++) = function;										//put opcode at the beginning
			strcpy(ch--, argv[argCnt + 1]);							//put key to get from server
			//printf("Sending to server %s\n", request);				//TBR
			writeKey(sockFd, request, strlen(request));				//write request to socket
			read(sockFd, msg, 1);									//It is of crucial importance that we first read the server's opcode.
			if (msg[0] != 'n') {									//Just keep in mind that server does not always responds with null
				readKey(sockFd, response + 1);						//terminated strings. An 'n' means 'key not found' and is the only
			}														//case where response is not null terminated
			response[0] = msg[0];
			switch (response[0]) {
				case 'f':											//key has been found
					ch = response + 1;
					printf("%s\n", ch);
					break;
				case 'n':											//key not found
					printf("\n");
					break;
				//default:											//unexpectable behaviour not recognized from protocol
					//errResponse();									//TBR??
			}
			argCnt += 2;											//increment arguments' counter
		}
		else if (strcmp(argv[argCnt], "put") == 0) {				//command is 'put'
			function = 'p';
			ch = request;
			*(ch++) = function;
			strcpy(ch, argv[argCnt + 1]);
			ch += strlen(ch) + 1;									//take advantage of ch so as to form one request
			strcpy(ch, argv[argCnt + 2]);
			writeKey(sockFd, request, strlen(request) + strlen(ch) + 1); // +1 added for \0 in second string
			argCnt += 3;
		}
		else {														//TERMINATE CONNECTION!
			msg[0] = 't';
			//msg[1] = msg[2] = 0;									//TBR
			writeKey(sockFd, msg, 1);
			argCnt = argc;											//to get off the loop
		}
	}
	close(sockFd);
	freeaddrinfo(servinfo);
	return 0;
}
