#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <string.h>

struct plane * buffer;

/*To create a queue*/
int queue_init(int size){
    buffer = malloc(sizeof(struct plane * )*size);
    return 0;
}

/* To Enqueue an element*/
int queue_put(struct plane* x) {
    pthread_mutex_lock(&mut_id);
    while ()
        pthread_cond_wait(&no_lleno, &mut_id);
    buffer[pos] = plane[i];
    pos = (pos + 1) % MAX_BUFFER;
    n_elementos ++;
    pthread_cond_signal(&no_vacio);
    pthread_mutex_unlock(&mut_id);
    return 0;
}


/* To Dequeue an element.*/
struct plane* queue_get(void) {
    return NULL;
}

/*To check queue state*/
int queue_empty(void){
    return 0;
}

/*To check queue state*/
int queue_full(void){
    return pos==MAX_BUFFER;
}

/*To destroy the queue and free the resources*/
int queue_destroy(void){
    free(buffer);
    return 0;
}
