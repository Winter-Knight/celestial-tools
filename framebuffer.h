#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "input.h"

class Framebuffer {
public:
	void Init();
	void Use();
	void Stop();
	void CheckForClicks(InputHandler * input);

	GLuint framebuffer;
};






#endif // FRAMEBUFFER_H
