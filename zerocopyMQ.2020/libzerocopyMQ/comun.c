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

int getQueue(char *buf, const char *cola, char *message){
    char c = 'G';
    buf[0] = c;
    buf[1] = *cola;
    buf[2] = *message;
    buf[3] = '\0';
    
    return 0;
}

int putQueue(char *buf, const char *cola, char *message){
    char c = 'P';
    buf[0] = &c;
    buf[1] = *cola;
    buf[2] = *message;
    buf[3] = '\0';

    return 0;
}



