#include "libf.h"
#include "keyvalue.h"
#include <pthread.h>

extern struct data *pHead;

void sig_chld(int signo) {
	while (waitpid(-1, NULL, WNOHANG) > 0) {}
	//signal(SIGCHLD, sig_chld);
}

int main(int argc, char **argv) {
	void *act(void *);
	int sParent, sChild;													//sParent is the socket descriptor used to make server reachable
	socklen_t clients_queue;												//while sChild is used for each client that successfully connects
	struct sockaddr_in cliaddr, servaddr;
	pthread_t tid;

	//if (argc != 2) {errArgs(); return 0;}									//TBR??
	pHead = NULL;															//initilization for linked list

	sParent = socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));
	bind(sParent, (struct sockaddr *) &servaddr, sizeof(struct sockaddr_in));
	listen(sParent, 5);
	signal(SIGCHLD, sig_chld);
	while (1) {																//until power failure or something :P
		clients_queue = sizeof(cliaddr);
		sChild = accept(sParent, (struct sockaddr *) &cliaddr, &clients_queue); //accept a new connection
		pthread_create(&tid, NULL, &act, (void *)sChild);
		//pid = fork();
		//if (pid == 0) {
			
			//exit(0);
		//}
		//else close(sChild);
	}
	//server closes
	close(sParent);															//idealists go from here on
	delKeys();
	return 0;
}


void *act(void *arg) {
	char request[65535], *ch, *ch2, *fin;								//fin is used to detect a finished command
	int sChild = arg;
	while (readKey(sChild, request) > 0) {								//while there is something to read
		fin = request;													//Usually the whole command is read at once with readKey so this
		do {							//is where fin comes into action
										//printf("Next opcode to be read: %s\n", fin);				//TBR
			ch = fin + 1;
			if (*fin == 'p') {											//Command to to execute is 'put'
				ch2 = ch + strlen(ch) + 1;								//ch2 is just a temporary char* that designates the beginning of
				put(ch, ch2);											//the value. Same goes for ch and the key respectively.
				fin = ch2 + strlen(ch2) + 1;							//Increase fin's offset to point to next opcode.
			}
			else if (*fin == 'g') {										//Command to to execute is 'get'
				ch2 = get(ch);											//Trying to find key. It is possible that ch2 gets NULL returned
				if (ch2 != NULL) {										//Key found!
																		//printf("Answering: %s\n", ch2);						//TBR
					*(--ch2) = 'f';										//Actually client's command opcode is overwritten with opcode 'f'
					writeKey(sChild, ch2, strlen(ch2));					//in request string.
				}
				else {													//key not found
					writeChar(sChild, 'n');								//just send opcode 'n'
				}
				fin = ch + strlen(ch) + 1;								//Increase fin's offset
			}
			else {														//unsupported protocol - terminate tcp connection
				close(sChild);
			}
			//if (*fin != 'p' && *fin != 'g') close(sChild);				//make sure invalid requests terminate connection
		} while (*fin == 'p' || *fin == 'g');
	}																	//readKey says there is nothing else to read
	pthread_detach(pthread_self());
	close(sChild);														//so terminate connection
	memset(request, 0, 65535);											//need to zero buffer
	return;
}
