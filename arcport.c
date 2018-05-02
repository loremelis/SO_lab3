#include "queue.c"
#include "decl.h"

#define NUM_TRACKS 1
#define ATERRIZAJE 0
#define DESPEGUE 1


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
    
	pthread_mutex_init(&pluto, NULL);
    pthread_cond_init(&no_lleno, NULL);
    pthread_cond_init(&no_vacio, NULL);
    
    pthread_create(&th1, NULL, (void *)jefe_pista, NULL);
    pthread_create(&th2, NULL, (void *)radar, NULL);
    pthread_create(&th3, NULL, (void *)torre_de_control, NULL);
    
	
	
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    pthread_mutex_destroy(&pluto);
    pthread_cond_destroy(&no_lleno);
    pthread_cond_destroy(&no_vacio);

    for (int i=0; i<n; i++) {
		free(planes_jefe[i]);
		free(planes_radar[i]);
	}
	
	free(planes_jefe);
	free(planes_radar);
	queue_destroy();
	print_banner();

    return 0;
}



void jefe_pista(int n){

	planes_jefe = malloc(sizeof(struct plane *)*n);
    
    for (int i=0; i<n; i++){
        planes_jefe[i] = malloc(sizeof(plane));
        planes_jefe[i]->id_number = disp_id++;
        planes_jefe[i]->time_action = (float)rand()/RAND_MAX*5;
        planes_jefe[i]->action = DESPEGUE;
        planes_jefe[i]->last_flight = 0;
        
        queue_put(planes_jefe[i]);

    }
    
    
    pthread_exit(0);
}



void radar(int m){

    planes_radar = malloc(sizeof(struct plane *)*m);;
    
    for (int i=0; i<m; i++){
        planes_radar[i] = malloc(sizeof(plane));
        planes_radar[i]->id_number = disp_id++;
        planes_radar[i]->time_action = -1;
        planes_radar[i]->action = ATERRIZAJE;
        planes_radar[i]->last_flight = 0;
        
        queue_put(planes_radar[i]);
    }
    pthread_exit(0);
}

void torre_de_control() {
	plane * pollo;
	int i=0;
	
	while (!queue_empty()) {
		pollo = queue_get();
		printf("Aereo %i -esimo esegue per %i\n",i++,pollo->time_action);
		sleep(pollo->time_action);
	}
}

