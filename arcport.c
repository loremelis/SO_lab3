#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include "plane.h"

#define NUM_TRACKS 1

#define ATERRIZAJE 0
#define DESPEGUE 1
#define MAX_BUFFER 1024

pthread_mutex_t mut_id;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;

int n_elementos;
struct plane * buffer[MAX_BUFFER];
int disp_id = 0;
int n,m;

void jefe_pista();
void radar();
void torre_de_control();
void print_banner();

typedef struct plane plane;

void print_banner()
{
    printf("\n*****************************************\n");
    printf("Welcome to ARCPORT - The ARCOS AIRPORT.\n");
    printf("*****************************************\n\n");
}


int main(int argc, char ** argv) {
    
    switch (argc){
        case 1:
            n = 4;
            m = 3;
            break;
        case 6:
            n = atoi(argv[1]);
            m = atoi(argv[3]);
            break;
        default:
            perror("Wrong number of argument");
            exit(1);
            //Errrore
            
    }
    
    
    pthread_t th1, th2, th3;
    
    pthread_mutex_init(&mut_id, NULL);
    pthread_cond_init(&no_lleno, NULL);
    pthread_cond_init(&no_vacio, NULL);

    
    pthread_create(&th1, NULL, (void *)jefe_pista, NULL);
    pthread_create(&th2, NULL, (void *)radar, NULL);
    pthread_create(&th3, NULL, (void *)torre_de_control, NULL);
    
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    pthread_mutex_destroy(&mut_id);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);

    print_banner();

    return 0;
}



void jefe_pista(int n){
    
    int pos = 0;
    plane * planes[n];
    
    for (int i=0; i<n; i++){
        planes[i] = malloc(sizeof(plane));
        planes[i]->id_number = disp_id++;
        planes[i]->time_action = -1;
        planes[i]->action = DESPEGUE;
        planes[i]->last_flight = 0;
        
        pthread_mutex_lock(&mut_id);
        while (n_elementos == MAX_BUFFER)
            pthread_cond_wait(&no_lleno, &mut_id);
        //buffer[pos] = plane[i];
        queue_put(planes[i]);
        //pos = (pos + 1) % MAX_BUFFER;
        //n_elementos ++;
        pthread_cond_signal(&no_vacio);
        pthread_mutex_unlock(&mut_id);
    }
    pthread_exit(0);
}



void radar(int m){
    
    int pos = 0;
    plane * planes[m];
    
    for (int i=0; i<m; i++){
        planes[i] = malloc(sizeof(plane));
        planes[i]->id_number = disp_id++;
        planes[i]->time_action = -1;
        planes[i]->action = ATERRIZAJE;
        planes[i]->last_flight = 0;
        
        pthread_mutex_lock(&mut_id);
        while (n_elementos == MAX_BUFFER)
            pthread_cond_wait(&no_lleno, &mut_id);
        queue_put(planes[i]);
        //pos = (pos + 1) % MAX_BUFFER;
        //n_elementos ++;
        pthread_cond_signal(&no_vacio);
        pthread_mutex_unlock(&mut_id);
    }
    pthread_exit(0);
}

void torre_de_control(){}

