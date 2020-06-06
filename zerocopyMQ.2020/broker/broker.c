#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>	
#include <pthread.h>



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

#define TAM 1024



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

		int s, leido; 
		char buf[TAM];
		struct thread* ts;
		ts = (struct thread*) arg;

		s = ts->s;

		struct cola *c;

	//	int iovcnt;
	//while((leido=recv(s, buf, TAM, MSG_WAITALL))){
	while ((leido=read(s, buf, TAM))>0) {
			

		//printf("Taille du buffer: %s\n", strlen(buf));
		//buf[0] = (char *)iov[1].iov_base;
		char op;
		char queue;

		char takeFirst[3];

		strncpy(takeFirst, buf, 2);
		op = takeFirst[0];
		queue = takeFirst[1];

		char* queueName = &queue;

		printf("Option chose: %c \n", op);

		//char* queueName;
		//strncat(queueName, buf+1, 1);
		printf("Name of the queue: %c\n", queue);

		//printf("Name of the queue: %s\n", queueName);
		char* message = (char *)malloc(sizeof(char *));

		int error = 0;
		switch (op)
		{
		case 'C':
			//dic_visit(ts->dic, imprimeDic);

			c = dic_get(ts->dic, &queue, &error);
			if(error < 0){
				//The dic doesn't exist
				printf("The dic doesn't exist\n");
				c = cola_create();
				dic_put(ts->dic, &queue, (void*)(struct cola *)cola_create());
							c = dic_get(ts->dic, &queue, &error);


				
				send(s, "1", 2*sizeof(char), 0);
			}else{
				//Dic exist
				printf("The dic already exist\n");
				send(s, "-1", 2*sizeof(char), 0);
			}
			/*if(dic_put(ts->dic, queueName, (void*)cola_create) <0){
				send(s, "0", 2*sizeof(char), 0);
			}else{
				send(s, "1", 2*sizeof(char), 0);
			}*/
			break;
		case 'D':
			c = dic_get(ts->dic, &queue, &error);
			
			if(error == -1){
				//The dic doesn't exist
				printf("The dic doesn't exist");
				send(s, "-1", 2*sizeof(char), 0);
			}else{
				printf("Try to remove dic\n");
				dic_remove_entry(ts->dic, &queue, removeDic);
				send(s, "1", 2*sizeof(char), 0);
			}
			break;
		case 'G':
		printf("Check queue: %c\n", queue);
			c = dic_get(ts->dic, &queue, &error);
			if(error == -1){
				//Doesn't exist
				send(s, "0", 2*sizeof(char), 0);
			}else{
				//@TODO
				message = cola_pop_front(c, &error);
				if(error == -1){
					printf("Nada por leer \n\n");
					//Nothing to popUP
					send(s, "-1", 2*sizeof(char), 0);
				}else{
					printf("Se manda la cola\n\n");
					char* msg = (char *)malloc(strlen(message+3*sizeof(char) ));
				//	printf("Message to send %s", message);
					msg[0] = '1';
					msg[1] = '0';
					/*int i;
					for(i = 0; i<strlen(message); i++){
						msg[i+2] = message[i];
					}
					msg[i+3] = '\0';*/
					strcat(msg, message);
					msg[strlen(msg)] = '\0';
					printf("Send: %s\n", msg);
					send(s, msg, strlen(msg), 0);
					free(message);
					free(msg);
				}
			}
			break;
		case 'P':
		printf("Check queue %c: ", queue);
			c = dic_get(ts->dic, &queue, &error);
			
			
			if(error < -1){
				//doesn't exist
				printf("Dic doesn't exist\n");
				send(s, "-1", 2*sizeof(char), 0);
			}else{
				printf("OK\n");
				strncpy(message, &buf[2], strlen(buf)-1);
				printf("Message: %s\n", message);
				printf("Cola %d\n", c);
				cola_push_back(dic_get(ts->dic,&queue,&error), message);

				dic_remove_entry(ts->dic, &queue, NULL);
				dic_put(ts->dic, &queue, c);

				send(s, "1", sizeof(char)*2, 0);
			}
			
			break;
		default:
			break;
		}
		//free(message);
	}
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

