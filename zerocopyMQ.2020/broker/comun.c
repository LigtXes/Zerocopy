/*
 * Incluya en este fichero todas las implementaciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */
#include "comun.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>



int createQueue(char *buf, const char *cola){
    
    char c = 'C';
    buf[0] = c;
    buf[1] = *cola;
    buf[2] = '\0';


    return 0;
}

int destroyQueue(char *buf, const char *cola){
    char c = 'D';
    buf[0] = c;
    buf[1] = *cola;
    buf[2] = '\0';

    return 0;
}

int putQueue(char *buf, const char *cola, char *message){
    char c = 'P';
    buf[0] = c;
    buf[1] = *cola;
    int i;
     printf("Message: %s\n", buf);
    for(i = 0; i<strlen(message); i++){
        buf[2+i] = message[i];
    }
    buf[i+3] = '\0';
    //strcat(buf, '\0');

    
    printf("Message: %s\n", buf);
    return 0;
}

int getQueue(char *buf, const char *cola){
    char c = 'G';
    buf[0] = c;
    buf[1] = *cola;
    buf[2] = '\0';

    return 0;
}



