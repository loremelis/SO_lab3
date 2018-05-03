#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <string.h>
#include <pthread.h>

/* just for print_buffer function: */
#define NUM_TRACKS 1
#define ATERRIZAJE 0
#define DESPEGUE 1

pthread_mutex_t pluto;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;


struct plane ** buffer;

int buffer_size = -1, free_pos = 0, first_pos = 0, n_elementos = 0;


/* To create a queue of ptr to planes.
 * Planes must be allocated from the caller */
int queue_init(int size){
	
    buffer = malloc(sizeof(struct plane * )*size);
	buffer_size = size;
	
	//printf("assig size %i\n", size);
	
	pthread_mutex_init(&pluto, NULL);
    pthread_cond_init(&no_lleno, NULL);
    pthread_cond_init(&no_vacio, NULL);
    
    printf("[QUEUE] Buffer initialized\n");
	
    return 0;
}

/* To Enqueue an element (productor) */
int queue_put(struct plane * x) {
    
	pthread_mutex_lock(&pluto);
    while (n_elementos == buffer_size)
        pthread_cond_wait(&no_lleno, &pluto); 	// no_lleno vale  0 perché è pieno il buffer.
												// quando è posto a 1 dal cons. si sblocca
	//printf("Non in sez critica - scrivo nel buffer\n");
    buffer[free_pos] = x;
	//printf("scritto\n");
    free_pos = (free_pos + 1) % buffer_size;
    n_elementos ++;
	//printf("Mando signal\n");
    pthread_cond_signal(&no_vacio); // setta a 1 perché il buffer non è vuoto
    //printf("Esco dall sc\n");
	pthread_mutex_unlock(&pluto);
    printf("[QUEUE] Storing plane with id %i \n", x->id_number);
    
	return 0;
}


/* To Dequeue an element (consumidor) */
struct plane * queue_get(void) {
    
	struct plane * pippo; //= malloc(sizeof());
	
	pthread_mutex_lock(&pluto);
    while (!n_elementos){
		//printf("\bWait queue...");
        pthread_cond_wait(&no_vacio, &pluto); 	// no_vacio vale  0 perché è vuoto il buffer.
												// quando è posto a 1 dal prod. si sblocca
		//printf(" end\n");
	}
    pippo = buffer[first_pos];
    first_pos = (first_pos + 1) % buffer_size;
    n_elementos --;
    pthread_cond_signal(&no_lleno); // setta a 1 perché il buffer non è più pieno
    pthread_mutex_unlock(&pluto);
    printf("[QUEUE] Getting plane with id %i \n", pippo->id_number);
	return pippo;
}

/*To check queue state*/
int queue_empty(void){
    return n_elementos==0;
}

/*To check queue state*/
int queue_full(void){
    return n_elementos==buffer_size;
}

/*To destroy the queue and free the resources*/
int queue_destroy(void){
    
    //printf("Distruggendo tutto\n");
	free(buffer);
	
	pthread_mutex_destroy(&pluto);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    
	return 0;
}


void print_buffer() {
	for (int i=first_pos; i<free_pos; i++){
		printf("----- AEREO -----\nid: %i\ntime_action: %i\naction: %s\n----- -----",buffer[i]->id_number,buffer[i]->time_action,buffer[i]->action==ATERRIZAJE?"ATT":(buffer[i]->action==DESPEGUE?"DECOLLO":"SCON"));
		
	}

}
