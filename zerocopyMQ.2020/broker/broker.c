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
struct diccionario* dic;


struct thread{
	long s;
	struct diccionario * d;
};


#define TAM 1024

void removeDic(char *c, void *v){
	cola_destroy(v, NULL);
}

void * servicio(void *arg){

		int s, leido; 
		char buf[TAM];
		struct thread *t;
		t=(struct thread *)arg;
		s = (int)t->s;

		struct cola *c;



	while ((leido=read(s, buf, TAM))>0) {
			

		printf("Taille du buffer: %d\n", strlen(buf));

		char op;
		char queueName;

		char takeFirst[3];

		strncpy(takeFirst, buf, 2);
		op = takeFirst[0];
		queueName = takeFirst[1];

		printf("Option chose: %c \n", op);

		//char* queueName;
		//strncat(queueName, buf+1, 1);

		printf("Name of the queue: %c\n", queueName);
		char* message = (char *)malloc(sizeof(char));

		int error;
		switch (op)
		{
		case 'C':
			dic_get(dic, queueName,&error);
			if(error < 0){
				//The dic doesn't exist
				dic_put(dic, queueName, (void *)cola_create());
				send(s, "1", 2*sizeof(char), 0);
			}else{
				//Dic exist
				send(s, "0", 2*sizeof(char), 0);
			}
			/*if(dic_put(dic, queueName, (void*)cola_create) <0){
				send(s, "0", 2*sizeof(char), 0);
			}else{
				send(s, "1", 2*sizeof(char), 0);
			}*/
			break;
		case 'D':
			dic_get(dic, queueName, &error);
			
			if(error == -1){
				//The dic doesn't exist
				send(s, "0", 2*sizeof(char), 0);
			}else{
				printf("Try to remove dic\n");
				dic_remove_entry(dic, queueName, removeDic);
				send(s, "1", 2*sizeof(char), 0);
			}
			break;
		case 'G':
			c = dic_get(dic, queueName, &error);
			if(error == -1){
				//Doesn't exist
				send(s, "0", 2*sizeof(char), 0);
			}else{
				//@TODO
				message = cola_pop_front(c, &error);
				if(error == -1){
					//Nothing to popUP
					send(s, "0", 2*sizeof(char), 0);
				}else{
					send(s, '0'+message, sizeof(message) + 2*sizeof(char), 0);
				}
			}
			strncpy(message, &buf[2], strlen(buf)-1);
			printf("%s\n", message);
			break;
		case 'P':
			c = dic_get(dic, queueName, &error);
			
			
			if(error == -1){
				//doesn't exist
				send(s, "0", 2*sizeof(char), 0);
			}else{
				strncpy(message, &buf[2], strlen(buf)-1);
				printf("%s\n", message);

				cola_push_back(c, message);

				dic_remove_entry(dic, queueName, NULL);
				dic_put(dic, queueName, c);

				send(s, "1", sizeof(char)*2, 0);
			}
			
			break;
		default:
			break;
		}
		free(message);		
	}
	close(s);
	return NULL;
}

int main(int argc, char *argv[]){

	int s, s_conec;
    unsigned int tam_dir;
   	int opcion=1;
	struct sockaddr_in dir, dir_cliente;
    struct cola* queue;
	struct thread *t;
	t = (struct thread *)malloc(sizeof(struct thread ));


	pthread_t thid;
	pthread_attr_t atrib_th;

    if(argc!=2) {
        fprintf(stderr, "Uso: %s puerto\n", argv[0]);
        return 1;
    }

	t->d = dic_create();

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
		if ((t->s=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
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
		pthread_create(&thid,
			&atrib_th,
			servicio, 
			(void *)(struct thread *)t
		);
	

    }

    close(s);

    return 0;
}
