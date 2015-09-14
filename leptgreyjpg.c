#include <stdio.h>
#include <jpeglib.h>
#include "leptsci.h"

// sample jpeg code - https://github.com/ellzey/libjpeg/blob/master/example.c

#define HT 60
#define WD 80

//static char hhead[] = "HTTP/1.1 200 OK\r\nContent-Type: image/x-png\r\n\r\n";

png_byte *rps[HT];
png_byte image[WD * HT * 2];
unsigned short *simage = (unsigned short *) image;

int main(int argc, char *argv[])
{
    int y;

    // for each row of pixels in the image
    for (y = 0; y < HT; y++)
        // hold each pixel, this fills rps with the image
        rps[y] = image + WD * 2 * y;
    if (leptopen() || leptget(simage))
        return -1;
    // close it, this is leptsci.c
    leptclose();

    // create a png struct
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        return -2;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
        return -3;
    }
    png_init_io(png_ptr, stdout);

    png_set_IHDR(png_ptr, info_ptr, WD, HT, 16, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_ADAM7 /*_NONE */ ,
      PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    png_set_swap(png_ptr);

    // write the image to the png
    png_write_image(png_ptr, rps);

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}
