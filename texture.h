#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SDL2/SDL_image.h>
#include "glprogram.h"

class Texture {
public:
	void Init(std::string filename);
	void InitETC2(const char * filename);
	void Bind(GLProgram * program, int location);
	
	GLuint textureBuffer;
};


inline SDL_Surface * LoadImage(const char * filename, unsigned int format = SDL_PIXELFORMAT_RGBA32)
{
	SDL_Surface * image, * tmpImage;

	tmpImage = IMG_Load(filename);

	if (!tmpImage) {
		printf("Error: Image not found: %s!\n", filename);
		return NULL;
	}

	if (tmpImage->format->format == format)
		image = tmpImage;
	else {
		image = SDL_ConvertSurfaceFormat(tmpImage, format, 0);
		SDL_FreeSurface(tmpImage);
	}

	return image;
}

#endif // TEXTURE_H
