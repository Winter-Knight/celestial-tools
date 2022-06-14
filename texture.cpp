#include "texture.h"

#include <sstream>

void Texture::Init(const char * filename)
{
	glGenTextures(1, &textureBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	SDL_Surface * image = LoadImage(filename);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	SDL_FreeSurface(image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Texture::Bind(GLProgram * program, int location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	std::stringstream ss;
	ss << "texture" << location;
	program->SetUniformi(ss.str().c_str(), location);
}
