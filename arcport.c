#include "header.h"

extern void print_buffer();

void print_banner()
{
    printf("\n*****************************************\n");
    printf("Welcome to ARCPORT - The ARCOS AIRPORT.\n");
    printf("*****************************************\n\n");
}


int main(int argc, char ** argv) {
	
	int n,m;

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
    
    
	queue_init(5);
    
    pthread_create(&th1, NULL, (void *)jefe_pista, &n);
    pthread_create(&th2, NULL, (void *)radar, &m);
    pthread_create(&th3, NULL, (void *)torre_de_control, NULL);
    
	printf("main -- tread cretated!\n");
	
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);


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



void jefe_pista(void * n_){
	
	int n = *(int*)n_;
	int t; //DEBUG

	planes_jefe = malloc(sizeof(struct plane *)*n);
    
	printf("jefe , n vale %i\n",n);
	
    for (int i=0; i<n; i++) {
		printf("Assegnato tempo %i",t = ((float)rand())/RAND_MAX*5);
        planes_jefe[i] = malloc(sizeof(struct plane));
        planes_jefe[i]->id_number = disp_id++;
        planes_jefe[i]->time_action = t;
        planes_jefe[i]->action = DESPEGUE;
        planes_jefe[i]->last_flight = 0;
        queue_put(planes_jefe[i]);
		printf("jefe  -- Messo aereo in coda.\n");
		print_buffer();

    }
    
    
    pthread_exit(0);
}



void radar(void * m_) {

	int m = *(int*)m_;
	
    planes_radar = malloc(sizeof(struct plane *)*m);;
    
    for (int i=0; i<m; i++){
        planes_radar[i] = malloc(sizeof(struct plane));
        planes_radar[i]->id_number = disp_id++;
        planes_radar[i]->time_action = ((float)rand())/RAND_MAX*5;
        planes_radar[i]->action = ATERRIZAJE;
        planes_radar[i]->last_flight = 0;
        queue_put(planes_radar[i]);
		printf("radar -- Messo aereo in coda.\n");
		print_buffer();
    }
    pthread_exit(0);
}

void torre_de_control() {
	
	struct plane * pollo;
	
	while (1)
	{
		// if (queue_empty()) sched_yield();
		pollo = queue_get();
		printf("Aereo %i  esegue per %i action: %s\n",pollo->id_number, pollo->time_action,ATERRIZAJE?"ATT":(DESPEGUE?"DECOLLO":"SCON"));
		sleep(pollo->time_action);
	}
}

