#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "plane.h"


struct plane ** buffer;
int buffer_size = -1;
int free_pos = 0, first_pos = 0, n_elementos;
pthread_mutex_t pluto;

int disp_id = 0;
int n,m;

typedef struct plane plane;

plane ** planes_jefe;
plane ** planes_radar;
pthread_cond_t no_lleno;
pthread_cond_t no_vacio;


void jefe_pista();
void radar();
void torre_de_control();
void print_banner();