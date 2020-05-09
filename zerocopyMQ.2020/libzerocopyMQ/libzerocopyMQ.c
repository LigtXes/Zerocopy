#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "zerocopyMQ.h"
#include "comun.h"

#define TAM 1024

int createMQ(const char *cola) {
    int s = socketConnection();

    char* buf;
    buf = (char *)malloc(strlen(cola+2*sizeof(char)));

    createQueue(buf, cola);

    send(s, buf, strlen(buf), 0);

    char response[4];
    read(s, response, 4*sizeof(char));
    return atoi(&response[0]);
}
int destroyMQ(const char *cola){
  /*  int s = socketConnection();
    char* buf;
        buf = NULL;

    destroyQueue(buf, cola);
    send(s, buf, strlen(buf), 0);
    write(s, buf, 4*sizeof(void));
    cola = buf[1];
    return atoi(cola); */
    return 1;
}

int put(const char *cola, const void *mensaje, uint32_t tam) {
   /* int s = socketConnection();
    char* buf;
    buf = NULL;
    putQueue(buf, cola, mensaje);
    send(s, buf, strlen(buf), 0);
    write(s, buf, 5*sizeof(void));
    cola = buf[1];
    return atoi(cola);*/
    return 1;
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
  /*  int s = socketConnection();
    char* buf;
    buf = NULL;

    getQueue(buf, cola, mensaje);
    send(s, buf, strlen(buf), 0);
    write(s, buf, 5*sizeof(void));
    cola = buf[1];
    return atoi(cola);*/
    return 1;
}

int socketConnection(){
    int s, leido;
	struct sockaddr_in dir;
	struct hostent *host_info;
    
	if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("error creando socket");
		return 1;
	}


    host_info=gethostbyname(getenv("BROKER_HOST"));


    dir.sin_addr=*(struct in_addr *)host_info->h_addr;
	dir.sin_port=htons(atoi(getenv("BROKER_PORT")));
	dir.sin_family=PF_INET;

    if (connect(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error en connect");
		close(s);
		return -1;
	}

    return s;
}
