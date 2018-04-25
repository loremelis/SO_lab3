#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include "plane.h"

#define NUM_TRACKS 1

#define ATERRIZAJE 0
#define DESPEGUE 1

pthread_mutex_t mut_id;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;

//int n_elementos;
//sem_t elementos;
//sem_t huecos;
//int buffer[MAX_BUFFER];

int disp_id = 0;


void jefe_pista();
void radar();
void torre_de_control();
void print_banner();

void print_banner()
{
    printf("\n*****************************************\n");
    printf("Welcome to ARCPORT - The ARCOS AIRPORT.\n");
    printf("*****************************************\n\n");
}


int main(int argc, char ** argv) {
    
    pthread_t th1, th2, th3;
    
    /*
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&no_lleno, NULL);
    pthread_cond_init(&no_vacio, NULL);
    */
    
    //sem_init(&elementos, 0, 0);
    //sem_init(&huecos, 0, MAX_BUFFER);
    
    pthread_create(&th1, NULL, jefe_pista, NULL);
    pthread_create(&th2, NULL, radar, NULL);
    pthread_create(&th3, NULL, torre_de_control, NULL);
    
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    
    //sem_destroy(&huecos);
    //sem_destroy(&elementos);
    
    /*
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);
    */
    
    

    print_banner();

    return 0;
}



void *jefe_pista(int n){
    
    int pos = 0;
    struct plane;
    
    for (int i=0; i<n; i++){
        plane = malloc(sizeof(plane));
        plane.id_number = disp_id++;
        plane.time_action = -1;
        plane.action = DESPEGUE;
        plane.last_flight = 0;
        
        sem_wait(&huecos);
        //queue_put
        //pos = (pos + 1) % MAX_BUFFER;
        //sem_post(&elementos);
    }
    pthread_exit(0);
}



void radar(int m){
    
    int pos = 0;
    struct plane;
    
    for (int i=0; i<m; i++){
        plane = malloc(sizeof(plane));
        plane.id_number = disp_id++;
        plane.time_action = -1;
        plane.action = DESPEGUE;
        plane.last_flight = 0;
    }
}
    
}

//void torre_de_control()
