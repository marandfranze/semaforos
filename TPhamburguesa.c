#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>
#define LIMITE 50

sem_t salero;
sem_t plancha;
sem_t horno;
int final;

struct semaforos {
	    sem_t sem_mezclar;
		sem_t sem_salar;
		sem_t sem_armarmedallones;
		sem_t sem_cocinarhamburguesa;
		sem_t sem_hornearpan;
		sem_t sem_cortarextras;
		sem_t sem_armarhamburguesa;

};

struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
};

struct parametro {
 	int equipo_param;
  	struct semaforos semaforos_param;
 	struct paso pasos_param[8];
};

void* imprimirAccion(void *data, char *accionIn) {

FILE* texto=fopen("nuevo.txt","a");
char equipo[3];
char linea1[110];
char linea2[110];
char linea3[110];
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)		
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
			sprintf(equipo,"%d",mydata->equipo_param);
			strcat(linea1,"\naccion del equipo");
			strcat(linea1,equipo);
			strcat(linea1,"-");
			strcat(linea1,mydata->pasos_param[i].accion);		
			strcat(linea1,"\n");
			fputs(linea1,texto);

		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		strcat(linea2,":::::::::::: ingredientes  :::::::::::::");
		strcat(linea2,":\n");
		fputs(linea2,texto);
		for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
					strcpy(linea3,"");
							strcat(linea3,mydata->pasos_param[i].ingredientes[h]);
							strcat(linea3,"\n");
							fputs(linea3,texto);
				}
			}
		}
	}
fclose(texto);
}



void* cortar(void *data) {
        //creo el nombre de la accion de la funcion
        char *accion = "cortar";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 3300000 );
        //doy la señal a la siguiente accion (cortar me habilita mezclar)
        sem_post(&mydata->semaforos_param.sem_mezclar);
	pthread_exit(NULL);
}
void* mezclar(void *data) {

        //creo el nombre de la accion de la funcion
        char *accion = "mezclar";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_mezclar);
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 4400000 );
        //doy la señal a la siguiente accion (mezclar me habilita a salar la mezcla)
	sem_post(&mydata->semaforos_param.sem_salar);
	pthread_exit(NULL);
}
void* salar(void *data) {
	sem_wait(&salero);
        //creo el nombre de la accion de la funcion
        char *accion = "salar";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_salar);
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 5500000 );
        //doy la señal a la siguiente accion (salar me habilita a armar los medallones)
        sem_post(&salero);
	sem_post(&mydata->semaforos_param.sem_armarmedallones);
	pthread_exit(NULL);
}
void* armarmedallones(void *data) {
//creo el nombre de la accion de la funcion
        char *accion = "armar";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_armarmedallones);
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1100000 );
        //doy la señal a la siguiente accion (armar medallones me habilita que pueda cocinar las hamburguesas)
	sem_post(&mydata->semaforos_param.sem_cocinarhamburguesa);
    	pthread_exit(NULL);
}
void* cocinarhamburguesa(void *data) {
	sem_wait(&plancha);
        //creo el nombre de la accion de la funcion
        char *accion = "cocinar";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_cocinarhamburguesa);
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 2000000 );
        //doy la señal a la siguiente accion (cortar me habilita mezclar)
	sem_post(&plancha);
	sem_post(&mydata->semaforos_param.sem_hornearpan);
    	pthread_exit(NULL);
}
void* hornearpan(void *data) {
	sem_wait(&horno);
        //creo el nombre de la accion de la funcion
        char *accion = "hornear";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_hornearpan);
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1100000 );
        //doy la señal a la siguiente accion (hornear me habilita a cortar la lechuga y el tomate)
	sem_post(&horno);
        sem_post(&mydata->semaforos_param.sem_cortarextras);
    	pthread_exit(NULL);
}
void* cortarextras(void *data) {

        //creo el nombre de la accion de la funcion
        char *accion = "cortarextras";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_cortarextras);
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 1100000 );
        //doy la señal a la siguiente accion (cortarextras me habilita a armar la hamburguesa completa)
	sem_post(&mydata->semaforos_param.sem_armarhamburguesa);
    	pthread_exit(NULL);
}

void* armarhamburguesa(void *data) {
//creo el nombre de la accion de la funcion
        char *accion = "armarHamburguesa";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_armarhamburguesa);
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
	usleep( 1010101 );
        //uso sleep para simular que que pasa tiempo
	printf("%d\n",final);
	final++;
	printf("%d\n",final);
	if(final==1){
		printf("\tGano el equipo %d felicitaciones\n" , mydata->equipo_param); 
		FILE* archivofinal=fopen("nuevo.txt","a");
		char Elganador[120];
		char equipo[2];
		sprintf(equipo,"%d",mydata->equipo_param);
		strcat(Elganador,"\nGano el equipo:");
		strcat(Elganador,equipo);
		strcat(Elganador,"\n");
		fputs(Elganador,archivofinal);
		fclose(archivofinal);

}
else{
		printf("\tPerdio el equipo %d\n" , mydata->equipo_param);

		FILE* texto=fopen("nuevo.txt","a");
		char perdedor[99];
		char equipo[4];
		sprintf(equipo,"%d",mydata->equipo_param);
		strcat(perdedor,"\nPerdio el equipo :");
		strcat(perdedor,equipo);
		strcat(perdedor,"\n");
		fputs(perdedor,texto);
		fclose(texto);
}
usleep( 2000000 );
pthread_exit(NULL);
}
void* ejecutarReceta(void *i){

		sem_t sem_mezclar;
		sem_t sem_cortarextras;
		sem_t sem_hornearpan;
		sem_t sem_armarmedallones;
		sem_t sem_salar;
		sem_t sem_cocinarhamburguesa;
		sem_t sem_armarhamburguesa;

	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;

	int p = *((int *) i);

	printf("Ejecutando equipo %d \n", p);
	struct parametro *pthread_data = malloc(sizeof(struct parametro));
	pthread_data->equipo_param = p;
	FILE* texto2=fopen("nuevo.txt","w+");
	remove("nuevo.txt");


FILE* texto=fopen("nuevo.txt","a");
		char concurso[110];
		char equipo[3];
		strcat(concurso,"\nBienvenido al mejor concurso de burguers:");
		strcat(concurso,"\n");
		fputs(concurso,texto);
		fclose(texto);

	
	FILE* receta=fopen("receta.txt","r");
	char separa[]="-";
	int lenght=9;
	char *tok;
	char linea[110];
	for (int i=0;i<lenght;i++){
		fgets(linea,110,receta);
		tok=strtok(linea,separa);
		strcpy(pthread_data->pasos_param[i].accion,tok);
		while(tok!=NULL){
			for(int l=0;l<4;l++){
				tok=strtok(NULL,separa);
				if(tok!=NULL){
					strcpy(pthread_data->pasos_param[i].ingredientes[l],tok);
				}
			}
		}
	}
	fclose(receta);
	
	
	
	
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_salar = sem_salar;
	pthread_data->semaforos_param.sem_armarmedallones = sem_armarmedallones;
	pthread_data->semaforos_param.sem_cocinarhamburguesa = sem_cocinarhamburguesa;
	pthread_data->semaforos_param.sem_cortarextras = sem_cortarextras;
	pthread_data->semaforos_param.sem_hornearpan= sem_hornearpan;
	pthread_data->semaforos_param.sem_armarhamburguesa = sem_armarhamburguesa;


	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armarmedallones),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cocinarhamburguesa),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cortarextras),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_hornearpan),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armarhamburguesa),0,0);

	int rc;

	rc = pthread_create(&p1,NULL,cortar,pthread_data);
	rc = pthread_create(&p2,NULL,mezclar,pthread_data);
	rc = pthread_create(&p3,NULL,salar,pthread_data);
	rc = pthread_create(&p4,NULL,armarmedallones,pthread_data);
	rc = pthread_create(&p5,NULL,cocinarhamburguesa,pthread_data);
	rc = pthread_create(&p6,NULL,hornearpan,pthread_data);
	rc = pthread_create(&p7,NULL,cortarextras,pthread_data);
	rc = pthread_create(&p8,NULL,armarhamburguesa,pthread_data);



	pthread_join (p1,NULL);
	pthread_join (p2,NULL);
	pthread_join (p3,NULL);
	pthread_join (p4,NULL);
	pthread_join (p5,NULL);
	pthread_join (p6,NULL);
	pthread_join (p7,NULL);
	pthread_join (p8,NULL);

	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	sem_destroy(&sem_armarmedallones);
	sem_destroy(&sem_cocinarhamburguesa);
	sem_destroy(&sem_cortarextras);
	sem_destroy(&sem_hornearpan);
	sem_destroy(&sem_armarhamburguesa);

	 pthread_exit(NULL);
}

int main ()
{
	sem_init(&salero,0,1);
	sem_init(&plancha,0,1);
	sem_init(&horno,0,1);

        int rc;

        int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));

        *equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;


        pthread_t equipo1;
	pthread_t equipo2;
	pthread_t equipo3;



        rc = pthread_create(&equipo1,NULL,ejecutarReceta,equipoNombre1);
	rc = pthread_create(&equipo2,NULL,ejecutarReceta,equipoNombre2);
	rc = pthread_create(&equipo3,NULL,ejecutarReceta,equipoNombre3);

        pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);

        pthread_exit(NULL);

	sem_destroy(&salero);
	sem_destroy(&plancha);
	sem_destroy(&horno);

}





