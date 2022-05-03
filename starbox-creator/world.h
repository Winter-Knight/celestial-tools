#ifndef WORLD_H
#define WORLD_H

#include "window.h"
#include "input.h"
#include "star.h"
#include "optionswindow.h"
#include "framebuffer.h"
#include "../twister-std.h"

class World {
public:
	World(Window * w, InputHandler * i) { window = w; input = i; }
	void Init();
	void Play();

private:
	void UpdateStars();

	Window * window;
	InputHandler * input;
	OptionsWindow * optionsWindow;
	
	Camera * camera;
	
	StarArray starArray;
	Twister twister;

	// Debugging
	GLProgram * debugProgram;
	Framebuffer * debugFramebuffer;
};

#endif // WORLD_H
