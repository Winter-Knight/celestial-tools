#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "input.h"

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

class Framebuffer {
public:
	void Init(int numAttachments, int numChannels, GLenum channelType, int width, int height);
	void Use();
	void Stop();
	void CheckForClicks(InputHandler * input);
	GLenum GetReadPixelFormat();
	void * GetData(int attachment);
	
	bool SetFormats(GLenum * internalFormat, GLenum * externalFormat);
	bool InitImages();

	GLuint framebuffer;
	GLuint renderbuffer;
	GLuint colorTextures[8];
	int numAttachments;
	int numChannels;
	GLenum channelType;
	int width, height;
};

#endif // FRAMEBUFFER_H
