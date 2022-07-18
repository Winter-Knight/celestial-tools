#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <FreeImage.h>
#include "glprogram.h"

class Texture {
public:
	void Init(std::string filename);
	void InitETC2(const char * filename);
	void Bind(GLProgram * program, int location);
	
	GLuint textureBuffer;
};

inline FIBITMAP * LoadImage(const char * filename)
{
	FIBITMAP * tmpImage, * image;

	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename);
	tmpImage = FreeImage_Load(fif, filename, 0);
	
	if (!tmpImage) {
		printf("Error: Image not found: %s!\n", filename);
		return NULL;
	}
	
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(tmpImage);
	printf("image filename: %s     fif: %d     format: %d\n", filename, fif, imageType);
	if (imageType != FIT_BITMAP)
		printf("WARNING: Image type for %s may not be supported. Image type: %d\n", filename, imageType);

	if (FreeImage_GetBPP(tmpImage) == 32)
		image = tmpImage;
	else {
		image = FreeImage_ConvertTo32Bits(tmpImage);
		FreeImage_Unload(tmpImage);
	}
	
	return image;
}
	
#endif // TEXTURE_H
