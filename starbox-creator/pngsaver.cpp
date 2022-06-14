#include <libpng16/png.h>

#include "pngsaver.h"

void SaveToPNG(const char * filename, int numChannels, int bitDepth, int width, int height, unsigned char * data)
{
	int color_type;
	switch(numChannels) {
		case 3:
			color_type = PNG_COLOR_TYPE_RGB;
			break;
		case 4:
			color_type = PNG_COLOR_TYPE_RGBA;
			break;
		default:
			printf("numChannels of %d not supported. Must be 3 or 4.\n", numChannels);
			return;
	}

	png_struct * pngStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_info * pngInfo = png_create_info_struct(pngStruct);

	FILE * fp = fopen(filename, "wb");
	png_init_io(pngStruct, fp);

	png_set_IHDR(pngStruct, pngInfo, width, height, bitDepth, color_type,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(pngStruct, pngInfo);

	for (int i = 0; i < height; i++)
		png_write_row(pngStruct, &data[i * width * numChannels * (bitDepth / 8)]);
	png_write_end(pngStruct, NULL);
	
	fclose(fp);
	png_destroy_write_struct(&pngStruct, &pngInfo);
}
