#include <cstdio>
#include <chrono>

#include <celestialworlds.h>
#include <SDL2/SDL.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "boilerplate.h"

celestial::World celestialWorld;
celestial::Camera celestialCamera;

namespace stdtime = std::chrono;

long long nowMicro()
{
	stdtime::system_clock::time_point tp = stdtime::system_clock::now();
	stdtime::system_clock::duration   d  = tp.time_since_epoch();
	stdtime::microseconds             us = stdtime::duration_cast<stdtime::microseconds> (d);
	return us.count();
}

void draw()
{
	celestialWorld.setTime(nowMicro());
	celestialCamera.setAspect(float(windowWidth) / float(windowHeight));
	celestialCamera.setViewMatrix(view);

	clearWindow();
	celestialWorld.Draw(celestialCamera);
	drawTerrain();
	swapWindow();
}

void mainLoop()
{
	SDL_Event event;
	int quit = 0;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
		}
		
		draw();
	}
}

int main(int argc, char ** argv)
{
	if (argc < 2)
		systemFile = "flips.txt";
	else
		systemFile = argv[1];

	initSDL();
	initGL();

	celestialWorld.loadSystemFile(systemFile);
	celestialCamera = celestialWorld.getCamera();
	celestialWorld.putCameraOnBody("Earth");

	mainLoop();

	quitGL();
	quitSDL();
	return 0;
}
