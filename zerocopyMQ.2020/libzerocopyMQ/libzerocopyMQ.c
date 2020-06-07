#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
 #include <sys/uio.h>

#include "zerocopyMQ.h"
#include "comun.h"

#define TAM 1024

int createMQ(const char *cola) {
    int s = socketConnection();
    //char* buf;
    //buf = (char *)malloc(strlen(cola+2*sizeof(char)));
    //createQueue(buf, cola);
    int iovcnt = 3; 
    struct iovec iov[iovcnt];

    char c[2];
    c[0] = 'C';
    c[1] = '\0';
    iov[0].iov_base = c;
    iov[0].iov_len = sizeof(c);

    char queue[8];
    strcpy(queue, cola);

    iov[1].iov_base = queue;
    iov[1].iov_len = sizeof(queue);



    char buf[TAM];
    char* np = "No Message\0";
    strcpy(buf, np);
    iov[2].iov_base = buf;
    iov[2].iov_len = sizeof(buf);    

	printf("Message in iov: %s\n", iov[0].iov_base);
	printf("Message in iov: %s\n", iov[1].iov_base);

    
       ssize_t se;
    //int size = strlen(c) + strlen(cola) + strlen(buf);
/*
    struct iovec pre[iovcnt];
		int a;
        int b;
        int d;

        a = strlen(c);
        b = strlen(cola);
        d = strlen(buf);
                printf("Error\n");

		pre[0].iov_base = &a; 
		pre[1].iov_base = &b; 
		pre[2].iov_base = &d; 
		pre[0].iov_len = sizeof (&a);
		pre[1].iov_len = sizeof (&b);
		pre[2].iov_len = sizeof (&d);
        printf("Error\n");

        writev(s, pre, iovcnt);

        char buf2[TAM];
        read(s, buf2, TAM); //Mensaje de espera
    */

    se = writev(s, iov, iovcnt);

    printf("%d\n", se);
    //send(s, buf, strlen(buf), 0);
    char response[4];
    read(s, response, 2*sizeof(char));
    
    return atoi(&response);
}

int destroyMQ(const char *cola){
    int s = socketConnection();
    
    int iovcnt = 3; 
    struct iovec iov[iovcnt];

    char c[2];
    c[0] = 'D';
    c[1] = '\0';
    iov[0].iov_base = c;
    iov[0].iov_len = sizeof(c);

    char queue[8];
    strcpy(queue, cola);
    iov[1].iov_base = queue;
    iov[1].iov_len = sizeof(queue);


       char buf[TAM];
    char* np = "No Message\0";
    strcpy(buf, np);
    iov[2].iov_base = buf;
    iov[2].iov_len = sizeof(buf);     
    

    ssize_t se;
    //int size = strlen(c) + strlen(cola) + strlen(buf);

    /*struct iovec pre[iovcnt];
		int a;
        int b;
        int d;

        a = strlen(c);
        b = strlen(cola);
        d = strlen(buf);
                printf("Error\n");

		pre[0].iov_base = &a; 
		pre[1].iov_base = &b; 
		pre[2].iov_base = &d; 
		pre[0].iov_len = sizeof (&a);
		pre[1].iov_len = sizeof (&b);
		pre[2].iov_len = sizeof (&d);
        printf("Error\n");

        writev(s, pre, iovcnt);

        char buf2[TAM];
        read(s, buf2, TAM); //Mensaje de espera
    
*/
    se = writev(s, iov, iovcnt);

    char response[2];
    read(s, response, 2*sizeof(char));
    //char op = response[0];
    //printf("%c\n", op);
    /*if(strcmp){
        return -1;
    }*/
    return atoi(&response); 
}

int put(const char *cola, const void *mensaje, uint32_t tam) {
    int s = socketConnection();

    int iovcnt = 3; 
    struct iovec iov[iovcnt];

    char c[2];
    c[0] = 'P';
    c[1] = '\0';
    iov[0].iov_base = c;
    iov[0].iov_len = sizeof(c);

    char queue[8];
    strcpy(queue, cola);
    iov[1].iov_base = queue;
    iov[1].iov_len = sizeof(queue);



    char buf[TAM];
    strcpy(buf, (char *)mensaje);
    iov[2].iov_base = buf;
    iov[2].iov_len = sizeof(buf);  
    

    ssize_t se;
    //int size = strlen(c) + strlen(cola) + strlen(buf);
/*
    struct iovec pre[iovcnt];
		int a;
        int b;
        int d;

        a = strlen(c);
        b = strlen(cola);
        d = strlen(mensaje);
                printf("Error\n");

		pre[0].iov_base = &a; 
		pre[1].iov_base = &b; 
		pre[2].iov_base = &d; 
		pre[0].iov_len = sizeof (&a);
		pre[1].iov_len = sizeof (&b);
		pre[2].iov_len = sizeof (&d);
        printf("Error\n");

        writev(s, pre, iovcnt);

        char buf2[TAM];
        read(s, buf2, TAM); //Mensaje de espera
    
*/
    se = writev(s, iov, iovcnt);

/*
    iovcnt = 2; 
    struct iovec iovResp[iovcnt];

    char res[3];

    iovResp[0].iov_base = res;
    iovResp[0].iov_len = sizeof(res);

    char message[TAM];
    iovResp[1].iov_base = message;
    iovResp[1].iov_len = sizeof(message);

    readv(s, iovResp, iovcnt); */


    char response[4];
    read(s, response, 2*sizeof(char));
    return atoi(&response);
}

int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
         int s = socketConnection();

    int iovcnt = 3; 
    struct iovec iov[iovcnt];


    char c[2];
    c[0] = 'G';
    c[1] = '\0';
    iov[0].iov_base = c;
    iov[0].iov_len = sizeof(c);

    char queue[8];
    strcpy(queue, cola);
    iov[1].iov_base = queue;
    iov[1].iov_len = sizeof(queue);


       char buf[TAM];
    char* np = "No Message\0";
    strcpy(buf, np);
    iov[2].iov_base = buf;
    iov[2].iov_len = sizeof(buf);  
    

    ssize_t se;
    //int size = strlen(c) + strlen(cola) + strlen(buf);

    /*struct iovec pre[iovcnt];
		int a;
        int b;
        int d;

        a = strlen(c);
        b = strlen(cola);
        d = strlen(buf);
                printf("Error\n");

		pre[0].iov_base = &a; 
		pre[1].iov_base = &b; 
		pre[2].iov_base = &d; 
		pre[0].iov_len = sizeof (&a);
		pre[1].iov_len = sizeof (&b);
		pre[2].iov_len = sizeof (&d);
        printf("Error\n");

        writev(s, pre, iovcnt);

        char buf2[TAM];
        read(s, buf2, TAM); //Mensaje de espera
    
*/
    se = writev(s, iov, iovcnt);
    char response[TAM];

    iovcnt = 2; 
    struct iovec iovResp[iovcnt];

    char res[3];

    iovResp[0].iov_base = res;
    iovResp[0].iov_len = sizeof(res);

    char message[TAM];
    iovResp[1].iov_base = message;
    iovResp[1].iov_len = sizeof(message);

    readv(s, iovResp, iovcnt);

    /*read(s, response, TAM);
        printf("Error\n");

        char * resp = (char *)malloc((strlen(response)-2) * sizeof(char));
    strncpy(resp, &response[2], strlen(response)-2);
    resp[strlen(resp)] = '\0';
    */
    uint32_t i = (uint32_t) (strlen((char *)iovResp[1].iov_base));
    printf("Tam: %d", i);
    tam = &i;
    *mensaje = (char *)malloc(strlen(iovResp[1].iov_base)); 

    //mensaje = &resp;
    strcpy(*mensaje, (char *)iovResp[1].iov_base);
    //strcat(*mensaje, '\0');
    //*mensaje = (void *)&resp;
    //strncpy(resp, response, 2);
    //free(resp);
    return atoi((int *)iovResp[0].iov_base);

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
