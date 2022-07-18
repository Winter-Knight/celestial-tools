#include <string>
#include <cstring>
#include <sstream>
#include <netinet/in.h>

#include "texture.h"

void Texture::Init(std::string filename)
{
	if (filename.length() > 4) {
		std::size_t dotPos = filename.find_last_of('.');
		if (dotPos < filename.length()) {
			std::string ext = filename.substr(dotPos);
			if (ext == ".pkm")
				return InitETC2(filename.c_str());
		}
	}

	FIBITMAP * image = LoadImage(filename.c_str());
	if (!image)
		return;
	
	glGenTextures(1, &textureBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image),
		0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	FreeImage_Unload(image);
//	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Texture::InitETC2(const char * filename)
{
	glGenTextures(1, &textureBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	
	FILE * fp = fopen(filename, "rb");

	char versionString[9];
	fread(versionString, 1, 8, fp);
	versionString[8] = 0;
	if (strncmp(versionString, "PKM 20", 6))
		printf("Expected file to start with 'PKM 20'. This file starts with %s.\nReading anyway.\n", versionString);
	
	int width = 0, height = 0;
	fread(&width, 2, 1, fp);
	width = ntohs(width);
	fread(&height, 2, 1, fp);
	height = ntohs(height);
	printf("width: %d     height: %d\n", width, height);
	
	unsigned int skyboxSize = width * height / 2;
	char * skyboxBuffer = new char[skyboxSize];
	fseek(fp, 16, SEEK_SET);
	fread(skyboxBuffer, 1, skyboxSize, fp);

	fclose(fp);
	
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB8_ETC2, width, height, 0, skyboxSize, skyboxBuffer);
	delete[] skyboxBuffer;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Texture::Bind(GLProgram * program, int location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, textureBuffer);
	std::stringstream ss;
	ss << "texture" << location;
	program->SetUniformi(ss.str().c_str(), location);
}
