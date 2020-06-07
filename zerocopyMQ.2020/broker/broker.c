#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>	
 #include <sys/uio.h>

#include <pthread.h>

#define TAM 10

#define MAGIC (('Q' << 24) + ('U' << 16) + ('E' << 8) + 'U')
struct cola {
    int magic;
    int nentradas;
    struct entrada *prim;
    struct entrada *ult;
};

struct thread{
	struct diccionario* dic;
	int s;
};

pthread_mutex_t count_mutex;

//#define TAM 1024



void imprimeQueue(void *v){
	if(v){
		printf("Message: %s\n", (char *)v);
	}
}

void imprimeDic(char *c, void *v){
	printf("Name of the queue in main thread: %s\n", c);
	cola_visit(v, imprimeQueue);
}

void deleteQueue(void *v){
	free(v);
}

void removeDic(char *c, void *v){
	cola_destroy(v, deleteQueue);
}


void * servicio(void *arg){

		int s;
		ssize_t leido; 
		//char buf[TAM];
		char realmente_tam_cola[TAM];
		char realmente_tam_mensaje[TAM];

		struct thread* ts;
		ts = (struct thread*) arg;

		s = ts->s;

		struct cola *c;

		

		int iovcnt = 3;
		/*
		struct iovec pre[iovcnt];
		int a; 
		int b;
		int d;
		pre[0].iov_base = &a; 
		pre[1].iov_base = &b; 
		pre[2].iov_base = &d; 
		pre[0].iov_len = sizeof (&a);
		pre[1].iov_len = sizeof (&b);
		pre[2].iov_len = sizeof (&d);

		leido=readv(s, pre, iovcnt);
		char* buf ="Siguiente";
		printf("A: %d, B: %d, C: %d", a,b,d);
		send(s, buf, sizeof(buf), 0);
		struct iovec iov[iovcnt];
		char foo[a];
		iov[0].iov_base = foo; 
		iov[0].iov_len = sizeof(foo);

		char boo[b];
		iov[1].iov_base = boo; 
		iov[1].iov_len = sizeof(boo);

		char soo[d];
		iov[2].iov_base = soo; 
		iov[2].iov_len = sizeof(soo);
		*/
		char foo[2];
		/*
	struct iovec iov[iovcnt];
		char foo[2];
		iov[0].iov_base = foo; 
		iov[0].iov_len = sizeof(foo);

		char boo[8];
		iov[1].iov_base = boo; 
		iov[1].iov_len = sizeof(boo);

		//char soo[TAM];
		iov[2].iov_base = soo; 
		iov[2].iov_len = sizeof(soo);

	ssize_t recv = 0;
	*/
			//struct iovec iov[2];

		//iov[0].iov_base = (char *)malloc(sizeof(char) * TAM);
	while((leido=read(s, foo, 2))>0){
		
	//while ((leido=read(s, buf, TAM))>0) {
		//printf("Read: %d\n", leido);
		
		//printf("Message in iov 0: %s\n", iov[0].iov_base);
		//printf("Message in iov 1: %s\n", (char *)iov[1].iov_base);

		//printf("Message in iov 2: %s\n", (char *)iov[2].iov_base);
		//char buf[iov[0].iov_len];
		//strcpy(buf, (char *)iov[0].iov_base);
		
		//printf("Taille du buffer: %s\n", strlen(buf));
		//buf[0] = (char *)iov[1].iov_base;
		char op;
		//char queue;

		//char takeFirst[2];

		//strncpy(takeFirst, buf, 2);
		//op = takeFirst[0];
		//queue[0] = buf[1];
		//queue = buf[1];

		dic_visit(ts->dic, imprimeDic);

		op = foo[0];
		while((read(s,realmente_tam_cola,TAM))>0){
			int tam_cola=atoi(realmente_tam_cola);
		char* queueName = (char *)malloc(sizeof(char)*atoi(realmente_tam_cola));
		while((recv(s,queueName,tam_cola,MSG_WAITALL))>0){
		//strcpy(queueName, (char *)iov[1].iov_base);
		//queueName[1] = '\0';

		printf("Option chose: %c \n", op);
		//char* queueName;
		//strncat(queueName, buf+1, 1);
		printf("Name of the queue: %s\n", queueName);

		//printf("Name of the queue: %s\n", queueName);
		//char* message = (char *)malloc(TAM * sizeof(char *));

		int error = 0;
		switch (op)
		{
		case 'C':
			//dic_visit(ts->dic, imprimeDic);

			c = dic_get(ts->dic, queueName, &error);

			cola_visit(c, imprimeQueue);
			cola_visit(dic_get(ts->dic,queueName, &error), imprimeQueue);	
			//cola_visit(c, imprimeQueue);
			//cola_visit(dic_get(ts->dic,&queue,&error), imprimeQueue);

			
			if(error < 0){
				//The dic doesn't exist
				c = cola_create();
				printf("The dic doesn't exist\n");
				int put = dic_put(ts->dic, queueName, c);
				
				send(s, "1", 2*sizeof(char), 0);
				close(s);
				return NULL;
			}else{
				//Dic exist
				printf("The dic already exist\n");
				send(s, "-1", 2*sizeof(char), 0);
				close(s);
				return NULL;
			}
			/*if(dic_put(ts->dic, queueName, (void*)cola_create) <0){
				send(s, "0", 2*sizeof(char), 0);
			}else{
				send(s, "1", 2*sizeof(char), 0);
			}*/
			break;
		case 'D':
			c = dic_get(ts->dic, queueName, &error);
			
			cola_visit(c, imprimeQueue);
			cola_visit(dic_get(ts->dic,queueName,&error), imprimeQueue);

			if(error == -1){
				//The dic doesn't exist
				printf("The dic doesn't exist");
				send(s, "-1", 2*sizeof(char), 0);
				close(s);
				return NULL;
			}else{
				printf("Try to remove dic\n");
				dic_remove_entry(ts->dic, queueName, removeDic);
				send(s, "1", 2*sizeof(char), 0);
				close(s);
				return NULL;
			}
			break;
		case 'G':
		printf("Check queue: %s\n", queueName);
			c = dic_get(ts->dic, queueName, &error);
			if(error == -1){
				//Doesn't exist
				//send(s, "0", 2*sizeof(char), 0);
				/*
				iovcnt = 2; 
    				struct iovec iovResp[iovcnt];
					iovResp[0].iov_base = res;
					iovResp[0].iov_len = sizeof(res);

					//char message[TAM];
					//message[0] = '\0';
					iovResp[1].iov_base = message;
					iovResp[1].iov_len = sizeof(message);

					writev(s, iovResp, iovcnt);*/
					printf("Nada por leer \n\n");
					//Nothing to popUP
					send(s, "-1", 2*sizeof(char), 0);
					close(s);
					return NULL;
			}else{
				//@TODO
				char *message =(char*) cola_pop_front(c, &error);
				if(error == -1){
					printf("Nada por leer \n\n");
					//Nothing to popUP
					send(s, "-1", 2*sizeof(char), 0);
					close(s);
					return NULL;
				}else{
					printf("Se manda la cola\n\n");
					sprintf(realmente_tam_mensaje,"%d",(int)strlen(message));

					iovcnt = 2; 
    				struct iovec iovResp[iovcnt];
					iovResp[0].iov_base = realmente_tam_mensaje;
					iovResp[0].iov_len = TAM;

					iovResp[1].iov_base = message;
					iovResp[1].iov_len = atoi(realmente_tam_mensaje);


				/*	char* msg = (char *)malloc(strlen(message+3*sizeof(char) ));
				//	printf("Message to send %s", message);
					msg[0] = '1';
					msg[1] = '0';
					*/
					/*
					int i;
					for(i = 0; i<strlen(message); i++){
						msg[i+2] = message[i];
					}
					msg[i+3] = '\0';
					*/ 
					/*
					strcat(msg, message);
					msg[strlen(msg)] = '\0';
					printf("Send: %s\n", msg);
					send(s, msg, strlen(msg), 0); 
					free(msg); 
					*/

					writev(s, iovResp, iovcnt);
				}
			}
			break;
		case 'P':
			while((read(s,realmente_tam_mensaje,TAM))>0){
				int tam_mensaje=atoi(realmente_tam_mensaje);
				char * mensaje=(char*)malloc(sizeof(char)*tam_mensaje);
				while((recv(s,mensaje,tam_mensaje,MSG_WAITALL))>0){
					c = dic_get(ts->dic, queueName, &error);
				
			cola_visit(c, imprimeQueue);
			cola_visit(dic_get(ts->dic,queueName, &error), imprimeQueue);

			if(error < -1){
				//doesn't exist
				printf("Dic doesn't exist\n");
				send(s, "-1", 2*sizeof(char), 0);
				close(s);
				return NULL;
			}else{
				
				printf("OK\n");
				printf("Message: %s\n", mensaje);
				cola_push_back(dic_get(ts->dic,queueName,&error),mensaje);
				send(s, "0", 2*sizeof(char), 0);
				close(s);
				return NULL;

				}
			}
			
			break;
		default:
			break;
		}
	}
	}
		//free(message);
	}
	}
	 printf("Read: %d\n",(int) leido);
	//printf("Message in iov: %s\n", iov[0].iov_base);


	close(s);
	return NULL;
}

int main(int argc, char *argv[]){

	int s, s_conec;
    unsigned int tam_dir;
   	int opcion=1;
	struct sockaddr_in dir, dir_cliente;

	struct thread* ts;
	ts = (struct thread*)malloc(sizeof(struct thread*));

	pthread_t thid;
	pthread_attr_t atrib_th;

    if(argc!=2) {
        fprintf(stderr, "Uso: %s puerto\n", argv[0]);
        return 1;
    }

	ts->dic = dic_create();

	pthread_attr_init(&atrib_th);
	pthread_attr_setdetachstate(&atrib_th, PTHREAD_CREATE_DETACHED);
	

    if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("error creando socket");
		return 1;
	}

    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion))<0){
                perror("error en setsockopt");
                return 1;
    }
	dir.sin_addr.s_addr=INADDR_ANY;
	dir.sin_port=htons(atoi(argv[1]));
	dir.sin_family=PF_INET;

    if (bind(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error en bind");
		close(s);
		return 1;
	}
	if (listen(s, 5) < 0) {
		perror("error en listen");
		close(s);
		return 1;
	}

//Broker is listening
    while(1){
        tam_dir=sizeof(dir_cliente);
		if ((s_conec=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
			perror("error en accept");
			close(s);
			return 1;
		}/* if(fork() == 0){
			close(s);
			servicio(s_conec);
			exit(0);
		}
		close(s_conec);
*/
		pthread_mutex_lock(&count_mutex);

		ts->s = s_conec;
		pthread_create(&thid,
			&atrib_th,
			servicio, 
			(void *)(struct thread *)ts
		);
		pthread_mutex_unlock(&count_mutex);

	

    }

    close(s);

    return 0;
}

