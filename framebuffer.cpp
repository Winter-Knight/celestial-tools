#include <cstdio>

#include "framebuffer.h"

void Framebuffer::Init(int numA, int numC, GLenum channelT, int w, int h)
{
	numAttachments = numA;
	numChannels = numC;
	channelType = channelT;
	width = w;
	height = h;

	if (numAttachments > 8) {
		printf("This program only supports a maximum of 8 framebuffer attachments!\n");
		printf("Reducing number of attachments to 8!\n");
		numAttachments = 8;
	}

	glGenRenderbuffers(1, &renderbuffer);
	glGenTextures(numAttachments, &colorTextures[0]);

	if (!InitImages())
		return;
	
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	for (int i = 0; i < numAttachments; i++) {
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, colorTextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, colorTextures[i], 0);
	}

	GLenum drawBuffers[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
	                          GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
	glDrawBuffers(numAttachments, drawBuffers);

	Stop();
}

void Framebuffer::Use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Framebuffer::Stop()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Framebuffer::SetFormats(GLenum * internalFormat, GLenum * externalFormat)
{
	// When other types of attachments need to be supported, add support here
	if (numChannels == 3 && channelType == GL_FLOAT) {
		*internalFormat = GL_RGB32F;
		*externalFormat = GL_RGB;
	}
	else if (numChannels == 4 && channelType == GL_FLOAT) {
		*internalFormat = GL_RGBA32F;
		*externalFormat = GL_RGBA;
	}
	else if (numChannels == 3 && channelType == GL_UNSIGNED_BYTE) {
		*internalFormat = GL_RGB;
		*externalFormat = GL_RGB;
	}
	else if (numChannels == 4 && channelType == GL_UNSIGNED_BYTE) {
		*internalFormat = GL_RGBA;
		*externalFormat = GL_RGBA;
	}
	else if (numChannels == 1 && channelType == GL_INT) {
		*internalFormat = GL_R32I;
		*externalFormat = GL_RED_INTEGER;
	}
	else {
		printf("That numChannels/channelType combo is not currently supported. Add support in framebuffer.cpp\n");
		printf("Framebuffer not properly initialized!\n");
		return false;
	}
	return true;
}

// Initialize or re-intialize image and depth buffer only, with new size if requested
bool Framebuffer::InitImages()
{
	GLenum internalFormat;
	GLenum externalFormat;
	if (!SetFormats(&internalFormat, &externalFormat))
		return false;

	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	for (int i = 0; i < numAttachments; i++) {
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, colorTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, externalFormat, channelType, NULL);
	}
	return true;
}

GLenum Framebuffer::GetReadPixelFormat()
{
	switch(numChannels) {
		case 1:
			return GL_RED;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
	}
	return GL_NONE;
}

/*void Framebuffer::CheckForClicks(InputHandler * input)
{
	GLenum format = GetReadPixelFormat();

	glm::vec4 foutput(0.0f);
	glm::ivec4 ioutput(0);
	unsigned int boutput = 0;

	if (!SDL_GetRelativeMouseMode() && input->button == SDL_BUTTON_LEFT) {
		Use();
		
		// TODO: This only supports 3 floats or 3 ints. Bytes not tested.
		for (int i = 0; i < numAttachments; i++) {
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
			if (channelType == GL_FLOAT) {
				glReadPixels(input->xpos, input->ypos, 1, 1, format, channelType, &foutput[0]);
				printf("output%d: [%5.2f][%5.2f][%5.2f]     ", i, foutput.x, foutput.y, foutput.z);
			}
			else if (channelType == GL_INT) {
				glReadPixels(input->xpos, input->ypos, 1, 1, format, channelType, &ioutput[0]);
				printf("output%d: [%d][%d][%d]     ", i, ioutput.x, ioutput.y, ioutput.z);
			}
			else if (channelType == GL_UNSIGNED_BYTE) {
				glReadPixels(input->xpos, input->ypos, 1, 1, format, channelType, &boutput);
				printf("output%d: %0x     ", i, boutput);
			}
			Window::CheckGLError("glReadPixels");
		}
		printf("\n");

		Stop();
	}
} */

void * Framebuffer::GetData(int attachment)
{
	int size;
	if (channelType == GL_INT || channelType == GL_FLOAT)
		size = 4;
	else if (channelType == GL_UNSIGNED_BYTE)
		size = 1;
	else {
		printf("Framebuffer channelType is unknown format.\n");
		return NULL;
	}
	
	unsigned char * data = new unsigned char[width * height * numChannels * size];
	Use();
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
	glReadPixels(0, 0, width, height, GetReadPixelFormat(), channelType, data);
	printf("%d %d %04x %04x %p\n", width, height, GetReadPixelFormat(), channelType, data);
	Stop();
	return data;
}
