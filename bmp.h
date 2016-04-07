/*
Header file for bmp image utility

Copyright (C) 2015 Kyle Gagner
All rights reserved
*/

// include guard
#ifndef BMP_H
#define BMP_H

#define BMP_RGB    1
#define BMP_ARGB   2
#define BMP_RGBA   3
#define BMP_LITTLE 4
#define BMP_BIG    8

#define BMP_FORMAT 3
#define BMP_ENDIAN 12

// writes a bitmap file
// width - width of image
// height - height of image
// pitch - bytes per horizontal line in image
// flags - format and endianness flags
// dpi - print resolution
// bitmap - image data
// filename - location for bmp file
void BMP_WriteBitmap(int width, int height, int pitch, int flags, int dpi, unsigned char *bitmap, char *filename);

#endif
