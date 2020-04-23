/*
 * Incluya en este fichero todas las definiciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */

#ifndef COMUN_H
#define COMUN_H  1

int createQueue(void *buf[], char *cola);

int destroyQueue(void *buf[], char *cola);

int putQueue(void *buf[], char *cola, void *message);

int getQueue(void *buf[], char *cola);

#endif 