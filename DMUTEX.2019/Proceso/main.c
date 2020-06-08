/* DMUTEX (2009) Sistemas Operativos Distribuidos

GREG
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------*/

// Tipo opaco para ocultar la implementación
struct diccionario;

// Tipo de datos para una función que visita una entrada
typedef void (*func_entrada_dic_t) (char *clave, void *valor);

// Crea un diccionario.
// Devuelve una referencia a un diccionario o NULL en caso de error.
struct diccionario *dic_create(void);

// Destruye el diccionario especificado. Si tiene todavía entradas
// se invocará la función especificada como parámetro por cada una de ellas
// pasando como argumentos a la misma la clave y valor de la entrada.
// Si la aplicación no está interesada en ser notificada de las entradas
// existentes, debe especificar NULL en el parámetro de esta función.
// Devuelve 0 si OK y -1 si error.
int dic_destroy(struct diccionario *d, func_entrada_dic_t liberar_entrada);

// Permite recorrer todas las entradas de un diccionario.
// Se invocará la función especificada como parámetro por cada una de ellas
// pasando como argumentos a la misma la clave y valor de la entrada.
// Devuelve 0 si OK y -1 si error.
int dic_visit(const struct diccionario *d, func_entrada_dic_t visitar_entrada);

// Inserta en el diccionario indicado la entrada especificada por la clave
// y el valor. Almacena referencias (y no copias) de la clave y el valor.
// Devuelve 0 si OK y -1 si error (p.e. si duplicada).
int dic_put(struct diccionario *d, const char *clave, const void *valor);

// Retorna el valor asociado a una clave.
// Dado que cualquier valor es válido, devuelve en el tercer parámetro
// si se ha producido un error: 0 si OK y -1 si error.
void * dic_get(const struct diccionario *d, const char *clave, int *error);

// Elimina una entrada dada su clave. Si se recibe un valor distinto
// de NULL en el tercer parámetro, se invoca esa función como parte
// de la eliminación de la entrada.
// Devuelve 0 si OK y -1 si error.
int dic_remove_entry(struct diccionario *d, const char *clave, func_entrada_dic_t liberar_entrada);
/*---------------------------------------------------------------------------------*/
// definición de tipos
struct entrada {
    const char *clave;
    const void *valor;
};
#define MAGIC (('D' << 24) + ('I' << 16) + ('C' << 8) + 'T')
struct diccionario {
    int magic;
    int nentradas;
    struct entrada *coleccion;
};

// especificación de funciones internas
static int busca_entrada(const struct diccionario *d, const char *clave);
static void itera_dic(const struct diccionario *d,  func_entrada_dic_t iter);
static int check_dic(const struct diccionario *d);
/*---------------------------------------------------------------------------------*/
struct proceso{
  char nombre[80];
  int p_id;
  int socket;
  int port;
};
/*---------------------------------------------------------------------------------*/

struct proceso *proceso_create(void);
/*---------------------------------------------------------------------------------*/
int puerto_udp;

int cadenaEsNumero (char **cadena);
int cadenaANumero (char **cadena);
int scan (char **cadena);
char* subString (const char* input, int offset, int len, char* dest);
int main(int argc, char* argv[])
{



  int n_de_scanner;
  int port;
  char * port_cadena;
  char line[80];
  char l2[80];
  char proc[80];
  char proc2[80];
  int s;
  int leido;
  struct sockaddr_in  init_s;
  socklen_t var1;
  socklen_t var2;
  int s2;
  struct sockaddr_in init_s2;
  int port2;
  int num_procesos=0;
  int LC[10];
  int *LC2;
  int LC3[10];
  char PROCESOS[10][80];
  int proceso_ID;
  int tam_mensaje;
  void*mensaje;
  int i1;

  struct proceso * p;
  struct diccionario *dict;

  struct proceso * procesos[10];
  int error;
  int i;

/*---------------------------------------------------------------------------------*/


  if(argc<2)
  {
    //fprintf(stderr,"Uso: proceso <ID>\n");
    return 1;
  }
  if((s=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<0){
    return 1;
  }
  init_s.sin_addr.s_addr=INADDR_ANY;
  init_s.sin_port=0; /* auto */
  init_s.sin_family=PF_INET;

  if(bind(s,(struct sockaddr*)&init_s,sizeof(init_s))<0){
    close(s);return 1;
  }
  var1=sizeof(init_s);
  if(getsockname(s,(struct sockaddr*)&init_s,&var1)<0) return 1;
  else 
    puerto_udp=ntohs(init_s.sin_port);

  setvbuf(stdout,(char*)malloc(sizeof(char)*80),_IOLBF,80);
  setvbuf(stdin,(char*)malloc(sizeof(char)*80),_IOLBF,80);


  fprintf(stdout,"%s: %d\n",argv[1],puerto_udp);
  


/*---------------------------------------------------------------------------------*/
dict=dic_create();

/*---------------------------------------------------------------------------------*/


  for(;fgets(line,80,stdin);)
  {
    if(!strcmp(line,"START\n"))
      break;

    n_de_scanner= sscanf(line,"%[^:]: %s",PROCESOS[num_procesos],port_cadena);
    if(n_de_scanner==2){
    /* Habra que guardarlo en algun sitio */
    //printf("%s: %d\n",PROCESOS[num_procesos],port_cadena);
    if(cadenaEsNumero(&port_cadena)){
      port2=cadenaANumero(&port_cadena);
      //printf("%s: %d\n",PROCESOS[num_procesos],port2);
      dic_get(dict,PROCESOS[num_procesos],&error);
      if(error==-1){
        procesos[num_procesos]=proceso_create();

        s2=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

        init_s2.sin_addr.s_addr=INADDR_ANY;
        init_s2.sin_port=htons(port2);
        init_s2.sin_family=PF_INET;
        s2=0;
        bind(s2,(struct sockaddr*)&init_s2,sizeof(init_s2));

        procesos[num_procesos]->port=port2;
        procesos[num_procesos]->socket=s2;
        procesos[num_procesos]->p_id=num_procesos;
        
        strcpy(procesos[num_procesos]->nombre,proc);
        dic_put(dict,PROCESOS[num_procesos],procesos[num_procesos]);

          if(!strcmp(PROCESOS[num_procesos],argv[1]))
          { /* Este proceso soy yo */ 
            proceso_ID=num_procesos;
          }
        num_procesos++;

    }
  }
    }
  }
  


/*---------------------------------------------------------------------------------*/

  /* Inicializar Reloj */
  for(i=0;i<num_procesos;i++){
    LC[i]=0;
  }

/*---------------------------------------------------------------------------------*/
  /* Procesar Acciones */
  for(;fgets(line,80,stdin);)
  {
    if(!strcmp(line,"FINISH\n"))
      break;
    if(!strcmp(line,"GETCLOCK\n")){
        printf("%s: LC[",PROCESOS[proceso_ID]);
        for(i=0;i<num_procesos;i++){
          if(i==num_procesos-1){
            printf("%d",LC[i]);
          }else{
            printf("%d,",LC[i]);
          }
        }
        printf("]\n");
    }
    if(!strcmp(line,"RECEIVE\n")){
      tam_mensaje=sizeof(int)+(num_procesos)*sizeof(int);
      mensaje=malloc(tam_mensaje);
      if((leido=read(s,mensaje,tam_mensaje))>0){
        i1=((int*)mensaje)[0];
        printf("%s: RECEIVE(MSG,%s)\n",PROCESOS[proceso_ID],PROCESOS[i1]);
        
        for(i=0;i<num_procesos;i++){
          LC3[i]=((int*)mensaje)[(i+1)] ;
        }
        for(i=0;i<num_procesos;i++){
          if(LC3[i] > LC[i])
            LC[i]=LC3[i];
        }
        printf("%s: TICK\n",PROCESOS[proceso_ID]);
        LC[proceso_ID]++;
      }
      printf("%s: TICK\n",PROCESOS[proceso_ID]);

      }
    
    if(!strcmp(line,"EVENT\n")){
      LC[proceso_ID]++;
      printf("%s: TICK\n",PROCESOS[proceso_ID]);
    }
    if(strlen(line)>10){
    sscanf(line,"%s %s",l2,proc2);
    if(!strcmp(l2,"MESSAGETO")){
      l2[0]='\0';
      dic_get(dict,proc2,&error);
      if(error!=-1){
        LC[proceso_ID]++;
        printf("%s: TICK\n",PROCESOS[proceso_ID]);
        p=(struct proceso*)dic_get(dict,proc2,&error);
        var2=sizeof(init_s2);
        tam_mensaje=sizeof(int)+(num_procesos)*sizeof(int);
        LC2=(int*)malloc(sizeof(int)*num_procesos);
        for(i=0;i<num_procesos;i++){
          LC2[i]=LC[i];
        }
        mensaje=malloc(tam_mensaje);
        memcpy(mensaje+sizeof(int),LC2,(num_procesos)*sizeof(int));
        free(LC2);
        ((int*)mensaje)[0]=proceso_ID;

        init_s2.sin_addr.s_addr=INADDR_ANY;
        init_s2.sin_port=htons(p->port);
        init_s2.sin_family=PF_INET;
        sendto(s, mensaje, tam_mensaje, 0,(struct sockaddr *)&init_s2, var2);
        printf("%s: SEND(MSG,%s)\n",PROCESOS[proceso_ID],PROCESOS[p->p_id]);
      }
    }
    }
    }

/*---------------------------------------------------------------------------------*/
  close(s);
  return 0;
}

/*---------------------------------------------------------------------------------*/


struct proceso *proceso_create(void) {
    struct proceso *p = malloc(sizeof(struct proceso));
    if (!p) return NULL;
    return p;
}
/*---------------------------------------------------------------------------------*/

// implementación de funciones externas
struct diccionario *dic_create(void) {
    struct diccionario *d = malloc(sizeof(struct diccionario));
    if (!d) return NULL;
    d->magic=MAGIC;
    d->nentradas=0;
    d->coleccion=NULL;
    return d;
}
int dic_destroy(struct diccionario *d, func_entrada_dic_t liberar_entrada){
    if (check_dic(d)) return -1;
    if (liberar_entrada)
        itera_dic(d, liberar_entrada);
    free(d->coleccion);
    d->magic=0;
    free(d);
    return 0;
}
int dic_visit(const struct diccionario *d, func_entrada_dic_t visitar_entrada){
    if (check_dic(d)) return -1;
    if (visitar_entrada)
        itera_dic(d, visitar_entrada);
    return 0;
}
int dic_put(struct diccionario *d, const char *clave, const void *valor) {
    if (check_dic(d) || !clave || busca_entrada(d, clave)!=-1)  return -1;
    d->nentradas++;
    d->coleccion=realloc(d->coleccion, d->nentradas*sizeof(struct entrada));
    if (!d->coleccion) return -1;
    struct entrada e = {clave, valor};
    d->coleccion[d->nentradas-1] =  e;
    return 0;
}
void * dic_get(const struct diccionario *d, const char *clave, int *error){
    int ne, err=0;
    if ((check_dic(d)) || (!clave || (ne=busca_entrada(d, clave))==-1)) err=-1;
    if (error) *error=err;
    return (err==-1?NULL:(void *)d->coleccion[ne].valor);
}
int dic_remove_entry(struct diccionario *d, const char *clave,
        func_entrada_dic_t liberar_entrada) {
    int ne;
    if ((check_dic(d)) || !clave || (ne=busca_entrada(d, clave))==-1) return -1;
    if (liberar_entrada)
        liberar_entrada((char *)d->coleccion[ne].clave,
            (void *)d->coleccion[ne].valor);
    d->nentradas--;
    d->coleccion[ne] = d->coleccion[d->nentradas];
    d->coleccion=realloc(d->coleccion, d->nentradas*sizeof(struct entrada));
    return 0;
}

// implementación de funciones internas
static int busca_entrada(const struct diccionario *d, const char *clave){
    int i;

    if (!clave) return -1;
    for (i=0; (i<d->nentradas && strcmp(clave, d->coleccion[i].clave)!=0); i++);
    return (i==d->nentradas?-1:i);
}
static void itera_dic(const struct diccionario *d,  func_entrada_dic_t iter){
    for (int i=0; i<d->nentradas; i++)
        iter((char *)d->coleccion[i].clave, (void *)d->coleccion[i].valor);
}
static int check_dic(const struct diccionario *d){
    int res=0;
    if (d==NULL || d->magic!=MAGIC){
        res=-1; fprintf(stderr, "el diccionario especificado no es válido\n");
    }
    return res;
}
/*---------------------------------------------------------------------------------*/


int cadenaANumero (char **cadena)
{
  int d = 0, l = 0, i, j, b;
  while ((*cadena)[l] != '\0')
    {
      l++;
    }
  for (i = 0; i < l; i++)
    {
      b = 1;
      for (j = 1; j < (l - i); j++)
	{
	  b *= 10;
	}
      d += ((*cadena)[i] - 48) * b;
    }
  return d;
}

int cadenaEsNumero (char **cadena)
{
  int numero = 1;
  int i = 0;
  while ((*cadena)[i] != '\0')
    {
      if (!((*cadena)[i] >= 48 && (*cadena)[i] <= 57))
	{
	  numero = 0;
	  break;
	}
      i++;
    }
  if (i < 1)
    {
      numero = 0;
    }
  return numero;
}

char * cadenaAlReves(char *s, int l){
int i;
 char * r=(char*)malloc((l+2)*sizeof(char));
 for( i=0;i<l;i++){
  r[(l-(i+1))]=s[i];
 }
 r[l]='\0';
 return r;
}
char * numeroACadena(int n){
 char * s;
 s=(char*)malloc(sizeof(char));
 int i=0;
 s[i]=(char)((n%10)+48);
 while(n/10>0){
  i++;
  s=(char*) realloc(s,(i+2)*sizeof(char));
  n/=10;
  s[i]=(char)((n%10)+48);
 }
 s=cadenaAlReves(s,(i+1));
 s[(i+1)]='\0';
 return s;
}

int scan (char **cadena)
{
  char c;
  int i = 0;
  *cadena = (char *) malloc (sizeof (char));
  fflush(stdin);
  setbuf (stdin, NULL);
  while (c != '\n')
    {
      c = getc (stdin);
      if (c != '\n')
	{
	  (*cadena)[i] = c;
	  i++;
	  *cadena = (char *) realloc (*cadena, sizeof (char) * (i + 1));
	}
    }
  (*cadena)[i] = '\0';
  return i;
}
char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);

  if (offset + len > input_len)
  {
     return NULL;
  }

  strncpy (dest, input + offset, len);
  return dest;
}