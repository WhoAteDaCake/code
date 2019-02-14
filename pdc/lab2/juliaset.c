/** calculate julia set, based on http://lodev.org/cgtutor/juliamandelbrot.html.
 * this version calculates specified row range, to facilitate data-parallelisation.
 * Also oversamples and averages within each pixel to make it work harder on smaller images.
 */
void juliaset(int w, int h, unsigned char *image, int fromy, int toy) {
    //each iteration, it calculates: new = old*old + c, where c is a constant and old starts at current pixel
    double cRe, cIm;                   //real and imaginary part of the constant c, determinate shape of the Julia Set
    double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
    double zoom = 1, moveX = 0, moveY = 0; //you can change these to zoom and change position
    int maxIterations = 300; //after how much iterations the function should stop
	int x, y;
	// sub-sample?!
	int sw = 6, sh = 6;
    //pick some values for the constant c, this determines the shape of the Julia Set
    cRe = -0.7;
    cIm = 0.27015;

	//loop through every pixel
	for(x = 0; x < w; x++)
		for(y = fromy; y <= toy && y < h; y++)
		{
			int sx, sy;
			int total = 0, count = 0;
			for (sx = 0; sx < sw; sx++)
				for (sy = 0; sy < sh; sy++) 
				{
					//calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
					newRe = 1.5 * (x - w / 2 + 1.0*sx/sw ) / (0.5 * zoom * w) + moveX;
					newIm = (y - h / 2 + 1.0*sy/sh) / (0.5 * zoom * h) + moveY;
					//i will represent the number of iterations
					int i;
					//start the iteration process
					for(i = 0; i < maxIterations; i++)
					{
						//remember value of previous iteration
						oldRe = newRe;
						oldIm = newIm;
						//the actual iteration, the real and imaginary part are calculated
						newRe = oldRe * oldRe - oldIm * oldIm + cRe;
						newIm = 2 * oldRe * oldIm + cIm;
						//if the point is outside the circle with radius 2: stop
						if((newRe * newRe + newIm * newIm) > 4) break;
					}
					// colour...?
					if (i < maxIterations) {
						int value = 255*i/(maxIterations-1);
						total += value;
						count += 1;
					}
				}
			if (count==0)
				image[4*(x+w*y)] = image[4*(x+w*y)+1] = image[4*(x+w*y)+2] = 0;
			else
				image[4*(x+w*y)] = image[4*(x+w*y)+1] = image[4*(x+w*y)+2] = total/count;
			image[4*(x+w*y)+3] = 255;
		}
}
