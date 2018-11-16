#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

#define SRV_PORT 30999
#define SRV_IP "83.212.104.180"
#define SRV_MAX_QUEUE 5
#define MAX_BUF 63


 ssize_t writen(int fd, const void *vptr, size_t n)
 {
     size_t nleft;
     ssize_t nwritten;
     const char *ptr;
     ptr = vptr;
     nleft = n;
     while (nleft > 0) {
         if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
             if (nwritten < 0 && errno == EINTR)
                 nwritten = 0;   /* and call write() again */
             else
                 return (-1);    /* error */
          }
          nleft -= nwritten;
          ptr += nwritten;
     }
     return (n);
 }



 void errArgs() {
	 printf("Invalid argument syntax has been used for this executable!\n");
	 getchar();
 }

void errSocket() {
	printf("Error in socket call!\nCould not get socket descriptor!\n");
	getchar();
}

void errBind() {
	printf("Error in bind call!\nCould not get socket descriptor!\n");
	getchar();
}

void errRead() {
	printf("Error in reading!\n");
	getchar();
}

void errWrite() {
	printf("Error in writing!\n");
	getchar();
}

void errResponse() {
	printf("Invalid response from server!\n");
	getchar();
}

void errAddrInfo() {
	printf("Error calling getaddrinfo!\n");
	getchar();
}

int getRecBuf(int sfd) {
    int n;
    unsigned int m = sizeof(n);
    getsockopt(sfd, SOL_SOCKET, SO_RCVBUF, (void *)&n, &m);
    return n;
}

int getSndBuf(int sfd) {
    int n;
    unsigned int m = sizeof(n);
    getsockopt(sfd, SOL_SOCKET, SO_SNDBUF, (void *)&n, &m);
    return n;
}

int readKey(int fd, char *dest) {	// TO BE CHANGED - RETURN SOMETHING TO WHILE LOOP
     register int readCnt;
     const int REC_BUF = getRecBuf(fd);
     /*printf("Got into readKey!\n");*/
     do {
         readCnt = read(fd, dest, REC_BUF);
	 /*printf("Just read %d bytes!\nLast byte is: %d\n", readCnt, *(dest+readCnt-1));*/
         if(readCnt < 0) {
                    if(errno == EINTR) continue;
                    //errRead();		//TBR
					return readCnt;
                    }
         if(readCnt == 0) break;
     } while (*(dest-1) != '\n' && *(dest+readCnt-1) != 0 && readCnt>0);
	 return readCnt;
}

void writeKey(int fd, char *dest, int len) {  // TO BE CHANGED - RETURN SOMETHING TO WHILE LOOP - MAYBE NOT?
	register int writeCnt = 0;
	const int DLEN = len + 1;
	//printf("Ready to send: %s\n", dest);		//TBR
	while (writeCnt < DLEN) {
		writeCnt += write(fd, dest, DLEN - writeCnt);
		//printf("Just wrote %d bytes!\nLast byte is: %c\n", writeCnt, *(dest+writeCnt-2));		//TBR
		if (writeCnt < 0) {
			if (errno == EINTR) {
				writeCnt = 0;  continue;
			}
			//errWrite();						//TBR
			return;
		}
	}
}

void writeChar(int fd, char c) {
	//*(&c + 1) = 0;							//TBR
	//printf("Char to write: %s\n", &c);		//TBR
	while (write(fd, &c, 1) != 1) { if (errno == EINTR) continue; }
}
