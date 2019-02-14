/* writetiff.h
 * see writetiff.c 
 */

/* image contains 4 bytes/pixel, RGB(A) */
void writeTiffFile(const char *filename, int width, int height, unsigned char *image);
