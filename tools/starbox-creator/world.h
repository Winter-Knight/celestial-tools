#ifndef WORLD_H
#define WORLD_H

#include "window.h"
#include "input.h"
#include "star.h"
#include "optionswindow.h"
#include "framebuffer.h"
#include "texturerect.h"
#include "twister-std.h"

using namespace celestial;

class World {
public:
	World(Window * w, InputHandler * i) { window = w; input = i; }
	void Init();
	void Play();

private:
	void UpdateStars();
	void UpdatePreviewStarbox();

	Window * window;
	InputHandler * input;
	OptionsWindow * optionsWindow;
	
	Camera * camera;
	
	StarArray starArray;
	StarArrayDrawerBillboards starArrayDrawerBillboards;
	StarArrayDrawerSpheres starArrayDrawerSpheres;
	Twister twister;

	// Preview
	Framebuffer * previewFramebuffer;
	TextureRect textureRect;

	// Debugging
	GLProgram * debugProgram;
	Framebuffer * debugFramebuffer;

	// Save to PNG
	Framebuffer * pngFramebuffer;
};

#endif // WORLD_H
