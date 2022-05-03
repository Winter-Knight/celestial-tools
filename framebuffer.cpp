#include "framebuffer.h"

const unsigned int FB_WIDTH = 2048;

void Framebuffer::Init()
{
	GLuint renderbuffer;
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, FB_WIDTH, FB_WIDTH);

	GLuint colorTextures[3];
	glGenTextures(3, &colorTextures[0]);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	for (unsigned int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, colorTextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, FB_WIDTH, FB_WIDTH, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, colorTextures[i], 0);
	}

//	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	GLenum drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, drawBuffers);

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

void Framebuffer::CheckForClicks(InputHandler * input)
{
	if (!SDL_GetRelativeMouseMode() && input->button == SDL_BUTTON_LEFT) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glm::vec3 pos, uv, color;
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(input->xpos, input->ypos, 1, 1, GL_RGB, GL_FLOAT, &pos[0]);
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glReadPixels(input->xpos, input->ypos, 1, 1, GL_RGB, GL_FLOAT, &uv[0]);
		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glReadPixels(input->xpos, input->ypos, 1, 1, GL_RGB, GL_FLOAT, &color[0]);

		Window::CheckGLError("glReadPixels");
		
		printf("pos: [%f][%f][%f]     uv: [%f][%f][%f]     color: [%5.2f][%5.2f][%5.2f]\n",
			pos.x, pos.y, pos.z, uv.x, uv.y, uv.z, color.r, color.g, color.b);
	}
}
