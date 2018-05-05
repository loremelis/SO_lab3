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

    int buffer_size, i = 0;
    
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
			while (argv[1][i]!=0)
				if (!isdigit(argv[1][i++])) exit(-1);
            n = atoi(argv[1]);
			if (n<0) exit(-1);
			
			i=0;
			while (argv[3][i]!=0)
				if (!isdigit(argv[3][i++])) exit(-1);
            m = atoi(argv[3]);
			if (m<0) exit(-1);
			
			i=0;
			while (argv[2][i]!=0)
				if (!isdigit(argv[2][i++])) exit(-1);
            t_off = atoi(argv[2]);
			if (t_off<0) exit(-1);
			
			i=0;
			while (argv[4][i]!=0)
				if (!isdigit(argv[4][i++])) exit(-1);
            t_land = atoi(argv[4]);
			if (t_land<0) exit(-1);
			
			i=0;
			while (argv[5][i]!=0)
				if (!isdigit(argv[5][i++])) exit(-1);
            buffer_size = atoi(argv[5]);
			if (buffer_size<=0) exit(-1);
			
            break;
        default:
            perror("Wrong number of argument");
            exit(-1);
    }
    
    n_planes = n + m;
	
	if (n_planes)  {
		print_banner();
		
		/* Inizialization Queue and semaphores */
		queue_init(buffer_size);
		sem_init(&lock,0,1);
		
		/* Creation Thread */
		pthread_create(&th1, NULL, (void *)jefe_pista, &n);
		pthread_create(&th2, NULL, (void *)radar, &m);
		pthread_create(&th3, NULL, (void *)torre_de_control, NULL);
		
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
		
		/* Destroy the Queue and sem. */
		queue_destroy();
		sem_destroy(&lock);
	} else printf("-----\nNo planes -- airport will remain closed\n-----\n");
	
	return 0;
}


/* Jefe De Pista
 * Create n planes (Despuegue)
 * Insert in the Buffer
 * */
void jefe_pista(void * n_){
	
	int n = *(int*)n_;

    /* Allocation Memory */
	planes_jefe = malloc(sizeof(struct plane *)*n);
	
    for (int i=0; i<n; i++) {

		sem_wait(&lock);
		
        /* Inizialization planes */
        planes_jefe[i] = malloc(sizeof(struct plane));
        planes_jefe[i]->id_number = disp_id++;
		planes_jefe[i]->time_action = t_off;
        planes_jefe[i]->action = DESPEGUE;
        if (planes_jefe[i]->id_number == n_planes - 1) {
            planes_jefe[i]->last_flight = 1;
        } else planes_jefe[i]->last_flight = 0;
		sem_post(&lock);
        printf("[TRACKBOSS] Plane with id %i checked\n", planes_jefe[i]->id_number);
        
        /* Put the planes in the buffer */
        queue_put(planes_jefe[i]);
		
        printf("[TRACKBOSS] Plane with id %i ready to take off\n", planes_jefe[i]->id_number);
    }
    pthread_exit(0);
}

/* Radar
 * Create m planes (Aterrizaje)
 * Insert in the Buffer
 * */
void radar(void * m_) {

	int m = *(int*)m_;
	
    /* Allocation Memory */
    planes_radar = malloc(sizeof(struct plane *)*m);;
    
    
    for (int i=0; i<m; i++){
        
		sem_wait(&lock);
        /* Inizialization planes */
        planes_radar[i] = malloc(sizeof(struct plane));
		planes_radar[i]->id_number = disp_id++;
        planes_radar[i]->time_action = t_land;
        planes_radar[i]->action = ATERRIZAJE;
        if (planes_radar[i]->id_number == n_planes - 1)
            planes_radar[i]->last_flight = 1;
		else planes_radar[i]->last_flight = 0;
        sem_post(&lock);

        printf("[RADAR] Plane with id %i detected!\n", planes_radar[i]->id_number);
        
        queue_put(planes_radar[i]);
        
        printf("[RADAR] Plane with id %i ready to land\n", planes_radar[i]->id_number);
    }
    pthread_exit(0);
}

/* Torre de Control
 * Consume m + n planes
 * Take off the planes from the buffer
 * */
void torre_de_control() {

	struct plane * avion;
	int resume_fd;
	static int n_planes_processed = 0;

	while (1){
		
		if (queue_empty()) {
            printf("[CONTROL] Waiting for planes in empty queue\n");
		}
            
		/* Take the planes from the Buffer */
		avion = queue_get();
		n_planes_processed++;
        
        switch (avion->action) {
            case DESPEGUE:
                printf("[CONTROL] Putting plane with id %i in track\n",avion->id_number);
                break;
            case ATERRIZAJE:
                printf("[CONTROL] Track is free for plane with id %i\n",avion->id_number);
                break;
            default:
                break;
        }
        
        if (avion->last_flight == 1){
            printf("[CONTROL] After plane with id %i the airport will be closed\n",avion->id_number);
            sleep(avion->time_action);
            printf("[CONTROL] Plane %i took off after %i seconds\n",avion->id_number, avion->time_action);
            break;
        } /* else printf("last_flight bit: %i \n", avion->last_flight); */
        sleep(avion->time_action);
        printf("[CONTROL] Plane %i took off after %i seconds\n",avion->id_number, avion->time_action);
		
	} /* END while */
	
	printf("Airport Closed!\n");
	resume_fd = open("resume.air", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU|S_IRWXG|S_IROTH);
	dprintf(resume_fd,"\t Total number of planes processed: %i\n\t Numbers of planes landed:%i \n\t Numbers of planes taken off:%i\n",n_planes,n,m);
    dprintf(1,"\t Total number of planes processed: %i\n\t Numbers of planes landed:%i \n\t Numbers of planes taken off:%i\n",n_planes_processed,n,m);
    printf("\n*****************************************\n");
    printf("---> Thanks for your trust in us <---\n");
    printf("*****************************************\n\n");
    
	close(resume_fd);
	
    pthread_exit(0);
    
}
