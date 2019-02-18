/** artificial producer/consumer style parallel juliaset, with 
 * each item = one row, one producers and a pool of consumers.
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

/* small - for demo purposes */
int width = 1000;
int height = 1000;
/* memory for image */
unsigned char *image;

/* shared job "queue", actually a single-place buffer, similar to the circular buffer in lecture 7 */
typedef struct buffer
{
	int item;
	int hasItem;
	pthread_mutex_t lock;
	pthread_cond_t notfull;
	pthread_cond_t notempty;
} buffer_t;

/* create buffer.
 * like a constructor. 
 */
buffer_t *buffer_new()
{
	buffer_t *buf = (buffer_t *)malloc(sizeof(buffer_t));
	pthread_mutex_init(&(buf->lock), NULL);
	pthread_cond_init(&(buf->notfull), NULL);
	pthread_cond_init(&(buf->notempty), NULL);
	buf->hasItem = 0;
	return buf;
}

/* put an item into the buffer. 
 * blocks if buffer is full (i.e. occupied).
 * buffer to use is fist argument (like a method call).
 */
void buffer_put(buffer_t *buf, int item)
{
	pthread_mutex_lock(&(buf->lock));
	while (buf->hasItem)
	{
		pthread_cond_wait(&(buf->notfull), &(buf->lock));
	}
	buf->item = item;
	buf->hasItem = 1;
	pthread_cond_signal(&(buf->notempty));
	pthread_mutex_unlock(&(buf->lock));
}

/* get an item from the buffer. 
 * blocks if buffer is empty.
 * buffer to use is fist argument (like a method call).
 */
int buffer_get(buffer_t *buf)
{
	int item;
	pthread_mutex_lock(&(buf->lock));
	while (!buf->hasItem)
	{
		pthread_cond_wait(&(buf->notempty), &(buf->lock));
	}
	item = buf->item;
	buf->hasItem = 0;
	pthread_cond_signal(&(buf->notfull));
	pthread_mutex_unlock(&(buf->lock));
	return item;
}

/* one buffer for new tasks (rows) */
buffer_t *newJobs;
/* one buffer for completed tasks (rows) */
buffer_t *completedJobs;

void *drawJuliaset_worker_thread(void *arg)
{
	int id = (int)arg;
	/* get new job = next row, generate, post to completed jobs. Exit on -1 */
	while (1)
	{
		int row = buffer_get(newJobs);
		if (row < 0)
			break;
		juliaset(width, height, image, row, row);
		printf("worker %d completed row %d\n", id, row);
		buffer_put(completedJobs, row);
	}
}

/* thread to create all new jobs */
void *drawJuliaset_newJobs_thread(void *arg)
{
	int row;
	for (row = 0; row < height; row++)
	{
		printf("newJob %d\n", row);
		buffer_put(newJobs, row);
	}
}

/* thread to collect all completed jobs  */
void *drawJuliaset_completedJobs_thread(void *arg)
{
	int row;
	for (row = 0; row < height; row++)
	{
		/* just assuming they are all ok */
		int item = buffer_get(completedJobs);
		printf("completedJob %d (%d)\n", row, item);
	}
}

void drawJuliaset()
{
	int i;
	/* handle for each thread */
	pthread_t worker_threads[MAX_THREADS], newJobs_thread, completedJobs_thread;

	/* make newJobs and completedJobs buffers (first!) */
	newJobs = buffer_new();
	completedJobs = buffer_new();

	/* make newJobs and completedJobs threads */
	pthread_create(&newJobs_thread, NULL, drawJuliaset_newJobs_thread, NULL);
	pthread_create(&completedJobs_thread, NULL, drawJuliaset_completedJobs_thread, NULL);

	/* start t worker threads */
	for (i = 0; i < t; i++)
	{
		pthread_create(&(worker_threads[i]), NULL, drawJuliaset_worker_thread, (void *)i);
	}

	/* wait until completed jobs thread finishes */
	pthread_join(completedJobs_thread, NULL);
}

int main(int argc, char **argv)
{
	struct timeval start, end;
	/* threads as an argument (required) */
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s NTHREADS\n", argv[0]);
		exit(-1);
	}
	t = atoi(argv[1]);
	/* memory for image */
	image = (unsigned char *)malloc(width * height * 4);
	memset((void *)image, 0, width * height * 4);
	printf("Call juliaset\n");
	gettimeofday(&start, NULL);
	/* whole juliaset image */
	drawJuliaset();
	/* and the answer is... */
	if (gettimeofday(&end, NULL) == 0)
	{
		double elapsed = (end.tv_sec - start.tv_sec) + 0.000001 * (end.tv_usec - start.tv_usec);
		printf("Elapsed time %lf seconds\n", elapsed);
	}
	else
	{
		printf("Could not get time using clock_gettime\n");
	}
	/* write file */
	printf("Write juliaset.tiff\n");
	writeTiffFile("juliaset.tiff", width, height, image);
	return 0;
}
