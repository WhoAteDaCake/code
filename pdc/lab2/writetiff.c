/** library-less tiff file writer.
 * based on http://paulbourke.net/dataformats/tiff/
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void WriteHexString(FILE *fptr,const char *s)
{
   unsigned int i,c;
   char hex[3];

   for (i=0;i<strlen(s);i+=2) {
      hex[0] = s[i];
      hex[1] = s[i+1];
      hex[2] = '\0';
      sscanf(hex,"%X",&c);
      putc(c,fptr);
   }
}
void writeTiffFile(const char *filename, int nx, int ny, unsigned char *image) {
	int i, j, offset;
	FILE * fptr = fopen(filename, "wb"); 

	/* Write the header */
	WriteHexString(fptr,"4d4d002a");    /* Big endian & TIFF identifier */
	offset = nx * ny * 3 + 8;
	putc((offset & 0xff000000) / 16777216,fptr);
	putc((offset & 0x00ff0000) / 65536,fptr);
	putc((offset & 0x0000ff00) / 256,fptr);
	putc((offset & 0x000000ff),fptr);

	/* Write the binary data */
	for (j=0;j<ny;j++) {
		for (i=0;i<nx;i++) {
			//... calculate the RGB value between 0 and 255 ...
			fputc(image[4*(j*nx+i)],fptr);
			fputc(image[4*(j*nx+i)+1],fptr);
			fputc(image[4*(j*nx+i)+2],fptr);
		}
	}

	/* Write the footer */
	WriteHexString(fptr,"000e");  /* The number of directory entries (14) */

	/* Width tag, short int */
	WriteHexString(fptr,"0100000300000001");
	fputc((nx & 0xff00) / 256,fptr);    /* Image width */
	fputc((nx & 0x00ff),fptr);
	WriteHexString(fptr,"0000");

	/* Height tag, short int */
	WriteHexString(fptr,"0101000300000001");
	fputc((ny & 0xff00) / 256,fptr);    /* Image height */
	fputc((ny & 0x00ff),fptr);
	WriteHexString(fptr,"0000");

	/* Bits per sample tag, short int */
	WriteHexString(fptr,"0102000300000003");
	offset = nx * ny * 3 + 182;
	putc((offset & 0xff000000) / 16777216,fptr);
	putc((offset & 0x00ff0000) / 65536,fptr);
	putc((offset & 0x0000ff00) / 256,fptr);
	putc((offset & 0x000000ff),fptr);

	/* Compression flag, short int */
	WriteHexString(fptr,"010300030000000100010000");

	/* Photometric interpolation tag, short int */
	WriteHexString(fptr,"010600030000000100020000");

	/* Strip offset tag, long int */
	WriteHexString(fptr,"011100040000000100000008");

	/* Orientation flag, short int */
	WriteHexString(fptr,"011200030000000100010000");

	/* Sample per pixel tag, short int */
	WriteHexString(fptr,"011500030000000100030000");

	/* Rows per strip tag, short int */
	WriteHexString(fptr,"0116000300000001");
	fputc((ny & 0xff00) / 256,fptr);
	fputc((ny & 0x00ff),fptr);
	WriteHexString(fptr,"0000");

	/* Strip byte count flag, long int */
	WriteHexString(fptr,"0117000400000001");
	offset = nx * ny * 3;
	putc((offset & 0xff000000) / 16777216,fptr);
	putc((offset & 0x00ff0000) / 65536,fptr);
	putc((offset & 0x0000ff00) / 256,fptr);
	putc((offset & 0x000000ff),fptr);

	/* Minimum sample value flag, short int */
	WriteHexString(fptr,"0118000300000003");
	offset = nx * ny * 3 + 188;
	putc((offset & 0xff000000) / 16777216,fptr);
	putc((offset & 0x00ff0000) / 65536,fptr);
	putc((offset & 0x0000ff00) / 256,fptr);
	putc((offset & 0x000000ff),fptr);

	/* Maximum sample value tag, short int */
	WriteHexString(fptr,"0119000300000003");
	offset = nx * ny * 3 + 194;
	putc((offset & 0xff000000) / 16777216,fptr);
	putc((offset & 0x00ff0000) / 65536,fptr);
	putc((offset & 0x0000ff00) / 256,fptr);
	putc((offset & 0x000000ff),fptr);

	/* Planar configuration tag, short int */
	WriteHexString(fptr,"011c00030000000100010000");

	/* Sample format tag, short int */
	WriteHexString(fptr,"0153000300000003");
	offset = nx * ny * 3 + 200;
	putc((offset & 0xff000000) / 16777216,fptr);
	putc((offset & 0x00ff0000) / 65536,fptr);
	putc((offset & 0x0000ff00) / 256,fptr);
	putc((offset & 0x000000ff),fptr);

	/* End of the directory entry */
	WriteHexString(fptr,"00000000");

	/* Bits for each colour channel */
	WriteHexString(fptr,"000800080008");

	/* Minimum value for each component */
	WriteHexString(fptr,"000000000000");

	/* Maximum value per channel */
	WriteHexString(fptr,"00ff00ff00ff");

	/* Samples per pixel for each channel */
	WriteHexString(fptr,"000100010001");

	fclose(fptr);
}
