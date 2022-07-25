#ifndef WORLD_H
#define WORLD_H

#include "window.h"
#include "camera.hpp"
#include "skypanorama.h"
#include "input.h"
#include "celestial.h"
#include "framebuffer.h"

using namespace celestial;

class World {
public:
	World(Window * w, InputHandler * i) { window = w; input = i; }
	void Init(const char * systemFile);
	void Update(long long time);
	void DrawPerspective(int celestialID);
	Camera * GetCamera() { return camera; }

private:
	Window * window;
	InputHandler * input;

	Camera * camera;
	SkyPanorama * skypanorama;

	std::vector<Celestial *> celestials;

	Framebuffer * debugFramebuffer;
	GLProgram * debugProgram;
};

#endif // WORLD_H
