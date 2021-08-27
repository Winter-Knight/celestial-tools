#ifndef WORLD_H
#define WORLD_H

#include "window.h"
#include "camera.h"
#include "skybox.h"
#include "input.h"
#include "celestial.h"

class World {
public:
	World(Window * w, InputHandler * i) { window = w; input = i; }
	void Init(const char * systemFile);
	void Play();

private:
	Window * window;
	InputHandler * input;

	Camera * camera;
	Skybox * skybox;

	std::vector<Celestial *> celestials;

	Celestial * sun;
	Orbital * planet;
	Orbital * rosa, * shadow, * daisy;
};

#endif // WORLD_H
