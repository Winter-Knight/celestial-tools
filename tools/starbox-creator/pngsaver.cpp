#include <cstring>
#include <libpng16/png.h>

#include "pngsaver.h"

void SaveToPNG(const char * filename, int numChannels, int bitDepth, int width, int height, const unsigned char * data, const char * creationSettingsText)
{
	int color_type;
	switch(numChannels) {
		case 1:
			color_type = PNG_COLOR_TYPE_GRAY;
			break;
		case 2:
			color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
			break;
		case 3:
			color_type = PNG_COLOR_TYPE_RGB;
			break;
		case 4:
			color_type = PNG_COLOR_TYPE_RGBA;
			break;
		default:
			printf("numChannels of %d not supported. Must be 1-4.\n", numChannels);
			return;
	}

	png_struct * pngStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_info * pngInfo = png_create_info_struct(pngStruct);

	png_text pngText;
	memset(&pngText, 0, sizeof(pngText));
//	pngText.compression = PNG_ITXT_COMPRESSION_zTXt;
	pngText.compression = PNG_ITXT_COMPRESSION_NONE;
	pngText.key = (char *) "Creation Settings";
	pngText.text = (char *) creationSettingsText;
	pngText.itxt_length = strlen(creationSettingsText);

	FILE * fp = fopen(filename, "wb");
	png_init_io(pngStruct, fp);

	png_set_IHDR(pngStruct, pngInfo, width, height, bitDepth, color_type,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_set_text(pngStruct, pngInfo, &pngText, 1);
	png_write_info(pngStruct, pngInfo);
	for (int i = 0; i < height; i++)
		png_write_row(pngStruct, &data[i * width * numChannels * (bitDepth / 8)]);

	png_write_end(pngStruct, NULL);
	
	fclose(fp);
	png_destroy_write_struct(&pngStruct, &pngInfo);
}
