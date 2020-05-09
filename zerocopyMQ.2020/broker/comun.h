/*
 * Incluya en este fichero todas las definiciones que pueden
 * necesitar compartir el broker y la biblioteca, si es que las hubiera.
 */

#ifndef COMUN_H
#define COMUN_H  1

int createQueue(char *buf, const char *cola);

int destroyQueue(char *buf, const char *cola);

int putQueue(char *buf, const char *cola, char *message);

int getQueue(char *buf, const char *cola, char *message);

#endif 