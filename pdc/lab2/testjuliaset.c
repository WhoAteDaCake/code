/** unthreaded juliaset / writetiff test.
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

int width = 1000;
int height = 1000;
/* memory for image */
unsigned char *image;

void drawJuliaset() {
	/* whole juliaset image, rows 0-(height-1), inclusive */
	juliaset(width, height, image, 0, height-1);
}

int main(int argc, char**argv) {
	struct timeval start, end;
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
