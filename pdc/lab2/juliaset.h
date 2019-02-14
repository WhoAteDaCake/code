/** calculate julia set, based on http://lodev.org/cgtutor/juliamandelbrot.html.
 * this version calculates specified row range, to facilitate data-parallelisation.
 * Also oversamples and averages within each pixel to make it work harder on smaller images.
 */
void juliaset(int w, int h, unsigned char *image, int fromy, int toy);
