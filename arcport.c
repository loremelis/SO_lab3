#include "header.h"

extern void print_buffer();

void print_banner()
{
    printf("\n*****************************************\n");
    printf("Welcome to ARCPORT - The ARCOS AIRPORT.\n");
    printf("*****************************************\n\n");
}


int main(int argc, char ** argv) {
	
    int n,m, n_planes;

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
    
    n_planes = n + m;
    t_off = atoi(argv[3]);
    t_land = atoi(argv[5]);
    
    print_banner();
    
	queue_init(5);
    
    pthread_create(&th1, NULL, (void *)jefe_pista, &n);
    pthread_create(&th2, NULL, (void *)radar, &m);
    pthread_create(&th3, NULL, (void *)torre_de_control, &n_planes);
    
	//printf("main -- tread cretated!\n");
	
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);


    for (int i=0; i<n; i++) {
		free(planes_jefe[i]);
	}
    for (int i=0; i<m; i++) {
        free(planes_radar[i]);
    }

	
	free(planes_jefe);
	free(planes_radar);
	
    
	queue_destroy();
	
    return 0;
}



void jefe_pista(void * n_){
	
	int n = *(int*)n_;
	// int t; //DEBUG

	planes_jefe = malloc(sizeof(struct plane *)*n);
    
	//printf("jefe , n vale %i\n",n);
	
    for (int i=0; i<n; i++) {
		//printf("Assegnato tempo %i",t = ((float)rand())/RAND_MAX*5);
        planes_jefe[i] = malloc(sizeof(struct plane));
        planes_jefe[i]->id_number = disp_id++;
        planes_jefe[i]->time_action = t_off;
        planes_jefe[i]->action = DESPEGUE;
        planes_jefe[i]->last_flight = 0;
        printf("[TRACKBOSS] Plane with id %i checked\n", planes_jefe[i]->id_number);
        
        queue_put(planes_jefe[i]);
        
        printf("[TRACKBOSS] Plane with id %i ready to take off\n", planes_jefe[i]->id_number);
		//printf("jefe  -- Messo aereo in coda.\n");
		//print_buffer();

    }
    
    
    pthread_exit(0);
}



void radar(void * m_) {

	int m = *(int*)m_;
	
    planes_radar = malloc(sizeof(struct plane *)*m);;
    
    for (int i=0; i<m; i++){
        planes_radar[i] = malloc(sizeof(struct plane));
        planes_radar[i]->id_number = disp_id++;
        planes_radar[i]->time_action = t_land;
        planes_radar[i]->action = ATERRIZAJE;
        planes_radar[i]->last_flight = 0;
        printf("[RADAR] Plane with id %i detected!\n", planes_radar[i]->id_number);
        
        queue_put(planes_radar[i]);
        
        printf("[RADAR] Plane with id %i ready to land\n", planes_radar[i]->id_number);
		//printf("radar  -- Messo aereo in coda.\n");
		//print_buffer();
    }
    pthread_exit(0);
}

void torre_de_control(void * n_planes_) {
    
    int n_planes = *(int*)n_planes_;
    int i = 0;
	struct plane * pollo;
	
	while (1)
	{
		if (queue_empty())
            printf("[CONTROL] Waiting for planes in empty queue\n");

        pollo = queue_get();
        
        switch (pollo->action) {
            case DESPEGUE:
                printf("[CONTROL] Putting plane with id %i in track\n",pollo->id_number);
                break;
            case ATERRIZAJE:
                printf("[CONTROL] Track is free for plane with id %i\n",pollo->id_number);
                break;
            default:
                break;
        }
        
        // Cambiare
        if (i < n_planes - 2){
            i++;
        } else {
            printf("[CONTROL] After plane with id %i the airport will be closed\n",pollo->id_number);
            break;
        }
		//printf("Aereo %i  esegue per %i action: %s\n",pollo->id_number, pollo->time_action,ATERRIZAJE?"ATT":(DESPEGUE?"DECOLLO":"SCON"));
		sleep(pollo->time_action);
	}
    
    printf("Airport Closed!\n");
    pthread_exit(0);
    
}

