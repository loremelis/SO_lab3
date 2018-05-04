#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define NUM_TRACKS 1
#define ATERRIZAJE 0
#define DESPEGUE 1

pthread_t th1, th2, th3;

/* Pointers to arrays of Pointers to planes structures */
struct plane ** planes_jefe;
struct plane ** planes_radar;

/* Functions for threads */
void jefe_pista();
void radar();
void torre_de_control();

/* Welcome msg */
void print_banner();

/* Definition in queue.c */
extern void print_buffer();

/* DEBUG LEVEL, definition in queue.c */
extern int debug;
