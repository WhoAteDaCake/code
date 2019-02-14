/** worker-pool style parallel juliaset, with each job = one row.
 */
/* compile/link with writetiff.c */
#include "writetiff.h"
/* compile/link with juliaset.c */
#include "juliaset.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

int t; /* number of threads */
/* max number of threads supported */
#define MAX_THREADS 1000

int width = 1000;
int height = 1000;
/* memory for image */
unsigned char *image;

/* shared state - next un-rendered row */
int nexty = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

/* get next row to render, or -1 if none left */
int get_next_row() {
	int row = -1;
	pthread_mutex_lock(&m);
	if (nexty < height) {
		row = nexty;
		nexty++;
	}
	pthread_mutex_unlock(&m);
	return row;
}

void* drawJuliaset_thread(void *arg) {
	/* get next row and draw, repeatedly until none are left */
	while(1) {
		int row = get_next_row();
		if (row<0)
			break;
		juliaset(width, height, image, row, row);
	}
}

void drawJuliaset() {
	int i;
	/* handle for each thread */
	pthread_t threads[MAX_THREADS];

	/* start t threads */
	for (i=0; i<t; i++) {
		pthread_create(&(threads[i]), NULL, drawJuliaset_thread, NULL);
	}

	/* wait for threads to finish */
	for (i=0; i<t; i++) {
		pthread_join(threads[i], NULL);
	}
}

int main(int argc, char**argv) {
	struct timeval start, end;
	/* threads as an argument (required) */
	if (argc<2) {
		fprintf(stderr,"Usage: %s NTHREADS\n", argv[0]);
		exit(-1);
	}
	t = atoi(argv[1]);
	/* memory for image */
	image = (unsigned char*)malloc(width*height*4);
	memset((void*)image, 0, width*height*4);
	printf("Call juliaset\n");
	gettimeofday(&start, NULL);
	/* whole juliaset image */
	drawJuliaset();
	/* and the answer is... */
	if (gettimeofday(&end, NULL)==0) {
		double elapsed = (end.tv_sec-start.tv_sec)+0.000001*(end.tv_usec-start.tv_usec);
		printf("Elapsed time %lf seconds\n", elapsed);
	}
	else {
		printf("Could not get time using clock_gettime\n");
	}
	/* write file */
	printf("Write juliaset.tiff\n");
	writeTiffFile("juliaset.tiff", width, height, image);
	return 0;
}
