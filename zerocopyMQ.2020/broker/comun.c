/*
 * Incluya en este fichero todas las implementaciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */
#include "comun.h"
#include <stdio.h>



int createQueue(void *buf[], char *cola){
    char c = 'C';
    buf[0] = &c;
    buf[1] = cola;
    buf[2] = '\0';

    return 0;
}

int destroyQueue(void *buf[], char *cola){
    char c = 'D';
    buf[0] = &c;
    buf[1] = cola;

    return 0;
}

int getQueue(void *buf[], char *cola, void *message){
    char c = 'G';
    buf[0] = &c;
    buf[1] = cola;
    buf[2] = message;
    buf[3] = '\0';
    
    return 0;
}

int putQueue(void *buf[], char *cola, void *message){
    char c = 'P';
    buf[0] = &c;
    buf[1] = cola;
    buf[2] = message;
    buf[3] = '\0';

    return 0;
}



