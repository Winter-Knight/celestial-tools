#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <FreeImage.h>
#include "glprogram.h"

class Texture {
public:
	void Init(std::string filename);
	void InitETC2(const char * filename);
	void Enhance();
	void Bind(GLProgram * program, int location);
	
	GLuint textureBuffer;

	static std::string baseDir;
	static void resetBaseDir();
	static void assignBaseDir(const std::string & filename);

//	baseDir: Assigned by location of system file
//	baseDir: Current directory "./"
//	baseDir: On linux: /usr/share/celestialworlds/
};

inline FIBITMAP * LoadImage(const char * theFilename)
{
	FIBITMAP * tmpImage, * image;

	std::string filename;
	if (theFilename[0] != '/')
		// Relative path
		filename = Texture::baseDir + theFilename;
	else
		// Absolute path, don't modify
		filename = theFilename;
	
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename.c_str());
	tmpImage = FreeImage_Load(fif, filename.c_str(), 0);
	
	if (!tmpImage) {
		printf("Error: Image not found: %s!\n", filename.c_str());
		return NULL;
	}
	
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(tmpImage);
	printf("image filename: %s     fif: %d     format: %d\n", filename.c_str(), fif, imageType);
	if (imageType != FIT_BITMAP)
		printf("WARNING: Image type for %s may not be supported. Image type: %d\n", filename.c_str(), imageType);

	if (FreeImage_GetBPP(tmpImage) == 32)
		image = tmpImage;
	else {
		image = FreeImage_ConvertTo32Bits(tmpImage);
		FreeImage_Unload(tmpImage);
	}
	
	return image;
}
	
#endif // TEXTURE_H
