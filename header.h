#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>
#include <unistd.h>

#define NUM_TRACKS 1
#define ATERRIZAJE 0
#define DESPEGUE 1

pthread_t th1, th2, th3;

/* id planes */
int disp_id = 0;

/* Pointers to arrays of Pointers to planes structures */
struct plane ** planes_jefe;
struct plane ** planes_radar;

/* Functions for threads */
void jefe_pista();
void radar();
void torre_de_control();

void print_banner();