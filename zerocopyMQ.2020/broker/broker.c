#include <stdio.h>
#include "comun.h"
#include "diccionario.h"
#include "cola.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>


#define MAGIC (('Q' << 24) + ('U' << 16) + ('E' << 8) + 'U')
struct cola {
    int magic;
    int nentradas;
    struct entrada *prim;
    struct entrada *ult;
};

#define TAM 1024

int main(int argc, char *argv[]){

    int s, s_conec, leido; 
    unsigned int tam_dir;
   	int opcion=1;
    char buf[TAM];
	struct sockaddr_in dir, dir_cliente;
    struct cola* queue;

	struct diccionario* dic;

    if(argc!=2) {
        fprintf(stderr, "Uso: %s puerto\n", argv[0]);
        return 1;
    }


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
		}
        if ((s_conec=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
			perror("error en accept");
			close(s);
			return 1;
		}
        while ((leido=read(s_conec, buf, TAM))>0) {
			
			dic = dic_create();

			//Message format: 
			// FunctionCode + ColaChar + Message
            queue = cola_create();
            cola_push_back(queue, buf);

			/*if (write(s_conec, buf, leido)<0) {
				perror("error en write");
				close(s);
				close(s_conec);
				return 1;
			}*/
		}

    }

    close(s);

    return 0;
}
