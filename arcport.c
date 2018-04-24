#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include "plane.h"

#define NUM_TRACKS 1

#define ATERRIZAJE 0
#define DESPEGUE 1


void jefe_pista();
//void radar();
//void torre_de_control();

pthread_mutex_t mut_id;
int disp_id = 0;

void print_banner()
{
    printf("\n*****************************************\n");
    printf("Welcome to ARCPORT - The ARCOS AIRPORT.\n");
    printf("*****************************************\n\n");
}


int main(int argc, char ** argv) {
    
    

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
