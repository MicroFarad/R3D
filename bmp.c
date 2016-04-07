/*
Source file for bmp image utility

Copyright (C) 2015 Kyle Gagner
All rights reserved
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "bmp.h"

#define BYTES(x) (x)&0xFF,(x)>>8&0xFF,(x)>>16&0xFF,(x)>>24&0xFF

void BMP_WriteBitmap(int width, int height, int pitch, int flags, int dpi, unsigned char *bitmap, char *filename)
{
	int bytes;
	int r, g, b;
	switch(flags & BMP_FORMAT)
	{
		case BMP_ARGB:
			bytes=4;
			switch(flags & BMP_ENDIAN)
			{
				case BMP_LITTLE:
					r=2;
					g=1;
					b=0;
					break;
				case BMP_BIG:
					r=1;
					g=2;
					b=3;
					break;
				default:
					return;
			}
			break;
		case BMP_RGBA:
			bytes=4;
			switch(flags & BMP_ENDIAN)
			{
				case BMP_LITTLE:
					r=3;
					g=2;
					b=1;
					break;
				case BMP_BIG:
					r=0;
					g=1;
					b=2;
					break;
				default:
					return;
			}
			break;
		case BMP_RGB:
			bytes=3;
			switch(flags & BMP_ENDIAN)
			{
				case BMP_LITTLE:
					r=2;
					g=1;
					b=0;
					break;
				case BMP_BIG:
					r=0;
					g=1;
					b=2;
					break;
				default:
					return;
			}
			break;
		default:
			return;
	}
	int wpitch = width*3;
	if(wpitch % 4) wpitch += 4 - wpitch%4;
	int size_bitmap = wpitch*height;
	int size_total = 0x36 + size_bitmap;
	int res = round(39.3701 * dpi);
	unsigned char header[] = {
		'B', 'M',
		BYTES(size_total),
		0x00, 0x00,
		0x00, 0x00,
		0x36, 0x00, 0x00, 0x00,
		0x28, 0x00, 0x00, 0x00,
		BYTES(width),
		BYTES(height),
		0x01, 0x00,
		0x18, 0x00,
		0x00, 0x00, 0x00, 0x00,
		BYTES(size_bitmap),
		BYTES(res),
		BYTES(res),
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00};
	unsigned char *pixels = malloc(size_bitmap);
	memset(pixels, 0, size_bitmap);
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int poff = y*wpitch+x*3;
			int boff = y*pitch+x*bytes;
			pixels[poff+0] = bitmap[boff+b];
			pixels[poff+1] = bitmap[boff+g];
			pixels[poff+2] = bitmap[boff+r];
		}
	}
	FILE *fp = fopen(filename, "wb");
	fwrite(header, 1, 0x36, fp);
	fwrite(pixels, 1, size_bitmap, fp);
	fclose(fp);
	free(pixels);
}
