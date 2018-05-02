//#include "decl.h"
#include "queue.h"

/* To create a queue of ptr to planes.
 * Planes must be allocated from the caller */
int queue_init(int size){
	
    buffer = malloc(sizeof(struct plane * )*size);
	buffer_size = size;
	
    return 0;
}

/* To Enqueue an element (productor) */
int queue_put(struct plane * x) {
    
	pthread_mutex_lock(&pluto);
    while (n_elementos == buffer_size)
        pthread_cond_wait(&no_lleno, &pluto); 	// no_lleno vale  0 perché è pieno il buffer.
												// quando è posto a 1 dal cons. si sblocca
    buffer[free_pos] = x;
    free_pos = (free_pos + 1) % buffer_size;
    n_elementos ++;
    pthread_cond_signal(&no_vacio); // setta a 1 perché il buffer non è vuoto
    pthread_mutex_unlock(&pluto);
	return 0;
}


/* To Dequeue an element (consumidor) */
struct plane * queue_get(void) {
    
	struct plane * pippo; //= malloc(sizeof());
	
	pthread_mutex_lock(&pluto);
    while (!n_elementos)
        pthread_cond_wait(&no_vacio, &pluto); 	// no_vacio vale  0 perché è vuoto il buffer.
												// quando è posto a 1 dal prod. si sblocca
    pippo = buffer[first_pos];
    first_pos = (first_pos + 1) % buffer_size;
    n_elementos --;
    pthread_cond_signal(&no_lleno); // setta a 1 perché il buffer non è più pieno
    pthread_mutex_unlock(&pluto);
	
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
    free(buffer);
    return 0;
}
