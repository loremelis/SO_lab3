#include "header.h"

/* id planes */
int disp_id = 0;

/* number pf planes */
int n_planes = 0;
int n, m;

/* time planes */
int t_off = 0;
int t_land = 0;



void print_banner()
{
    printf("\n*****************************************\n");
    printf("Welcome to ARCPORT - The ARCOS AIRPORT.\n");
    printf("*****************************************\n\n");
}


int main(int argc, char ** argv) {

    int buffer_size;
    
    /* Control of the arguments */
    switch (argc){
        case 1:
            n = 4;
            m = 3;
            t_off = 2;
            t_land = 3;
            buffer_size = 6;
            break;
        case 6:
            n = atoi(argv[1]);
            m = atoi(argv[3]);
            t_off = atoi(argv[2]);
            t_land = atoi(argv[4]);
            buffer_size = atoi(argv[5]);
            break;
        default:
            perror("Wrong number of argument");
            exit(1);
    }
    
    n_planes = n + m;
    
    // Debug argomento
    if(debug == 4){
        printf("n:%i\n",n);
        printf("m:%i\n",m);
        printf("t_off:%i\n",t_off);
        printf("t_land:%i\n",t_land);
        printf("buffer size:%i\n",buffer_size);
    }
    
    print_banner();
    
    /* Inizialization Queue */
	queue_init(buffer_size);
    
    /* Creation Thread */
    pthread_create(&th1, NULL, (void *)jefe_pista, &n);
    pthread_create(&th2, NULL, (void *)radar, &m);
    pthread_create(&th3, NULL, (void *)torre_de_control, NULL);
    
    if (debug == 0)
        printf("main -- tread cretated!\n");
	
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    /* Free Memory */
    for (int i=0; i<n; i++) {
		free(planes_jefe[i]);
	}
    for (int i=0; i<m; i++) {
        free(planes_radar[i]);
    }
    free(planes_jefe);
	free(planes_radar);
	
    /* Destroy the Queue */
	queue_destroy();
	return 0;
}

/* Jefe De Pista
 * Create n planes (Despuegue)
 * Insert in the Buffer
 * */
void jefe_pista(void * n_){
	
	int n = *(int*)n_;
    int t; /* DEBUG */

   // sleep(10);
    
    /* Allocation Memory */
	planes_jefe = malloc(sizeof(struct plane *)*n);
    
	if (debug == 0)
        printf("jefe , n vale %i\n",n);
	
    for (int i=0; i<n; i++) {
        
		if (debug == 0)
            printf("Assegnato tempo %i",t = ((float)rand())/RAND_MAX*5);
        
        /* Inizialization planes */
        planes_jefe[i] = malloc(sizeof(struct plane));
        planes_jefe[i]->id_number = disp_id++;
        planes_jefe[i]->time_action = t_off;
        planes_jefe[i]->action = DESPEGUE;
        if (planes_jefe[i]->id_number == n_planes - 1){
            planes_jefe[i]->last_flight = 1;
			// printf("%i\n",planes_jefe[i]->last_flight);
        } else {
            planes_jefe[i]->last_flight = 0;
        }

        if (debug == 3){
            printf("N Planes: %i\n",n_planes);
            printf("Last flight: %i\n",planes_jefe[i]->last_flight);
        }
        
        printf("[TRACKBOSS] Plane with id %i checked\n", planes_jefe[i]->id_number);
        
        /* Put the planes in the buffer */
        queue_put(planes_jefe[i]);
        
        printf("[TRACKBOSS] Plane with id %i ready to take off\n", planes_jefe[i]->id_number);
        
		if (debug == 0)
            printf("jefe  -- Messo aereo in coda.\n");
		if (debug == 2)
            print_buffer();

    }
    pthread_exit(0);
}

/* Radar
 * Create m planes (Aterrizaje)
 * Insert in the Buffer
 * */
void radar(void * m_) {

	int m = *(int*)m_;
	
	// sleep(10);
	
    /* Allocation Memory */
    planes_radar = malloc(sizeof(struct plane *)*m);;
    
    
    for (int i=0; i<m; i++){
        
        /* Inizialization planes */
        planes_radar[i] = malloc(sizeof(struct plane));
        planes_radar[i]->id_number = disp_id++;
        planes_radar[i]->time_action = t_land;
        planes_radar[i]->action = ATERRIZAJE;
        if (planes_radar[i]->id_number == n_planes - 1)
            planes_radar[i]->last_flight = 1;
        planes_radar[i]->last_flight = 0;
        
        
        if (debug == 3)
            printf("Last flight: %i", planes_radar[i]->last_flight);
        printf("[RADAR] Plane with id %i detected!\n", planes_radar[i]->id_number);
        
        queue_put(planes_radar[i]);
        
        printf("[RADAR] Plane with id %i ready to land\n", planes_radar[i]->id_number);
        
		if (debug == 0)
            printf("radar  -- Messo aereo in coda.\n");
		if (debug == 2)
            print_buffer();
    }
    pthread_exit(0);
}

/* Torre de Control
 * Consume m + n planes
 * Take off the planes from the buffer
 * */
void torre_de_control() {

	struct plane * pollo;
	int resume_fd;
	
	while (1){
		
		if (queue_empty()) {
            printf("[CONTROL] Waiting for planes in empty queue\n");
		}
            
		/* Take the planes from the Buffer */
		pollo = queue_get();
		printf("Last flight: %i\n",pollo->last_flight);
        
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
        if (pollo->last_flight == 1){
            printf("[CONTROL] After plane with id %i the airport will be closed\n",pollo->id_number);
            sleep(pollo->time_action);
            printf("[CONTROL] Plane %i took off after %i seconds\n",pollo->id_number, pollo->time_action);
            break;
        } else printf("last_flight bit: %i \n", pollo->last_flight);
        sleep(pollo->time_action);
        printf("[CONTROL] Plane %i took off after %i seconds\n",pollo->id_number, pollo->time_action);

		if (debug == 0)
            printf("Aereo %i  esegue per %i action: %s\n",
				   pollo->id_number, pollo->time_action,ATERRIZAJE?"ATERRIZAJE":(DESPEGUE?"DESPEGUE":"UNKNOWN"));
		
	} /* END while */
	
	resume_fd = open("resume.air", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU|S_IRWXG|S_IROTH);
	dup2(resume_fd,1);
	
    printf("Airport Closed!\n");
    printf("\t Total number of planes processed: %i\n\t Numbers of planes landed:%i \n\t Numbers of planes taken off:%i\n",n_planes,n,m);
    printf("\n*****************************************\n");
    printf("---> Thanks for your trust in us <---\n");
    printf("*****************************************\n\n");
    
	close(resume_fd);
	printf("end");
	
    pthread_exit(0);
    
}

