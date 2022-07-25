#include <cstdio>

#include <glm/glm.hpp>
#include <celestialworlds.hpp>

#include "compat.h"

#include "timefuncs.h"

#include "player.h"
#include "terrain.h"

#ifndef DEFAULT_SYSTEM_FILE
	#define DEFAULT_SYSTEM_FILE "../sol.txt"
#endif

celestial::World celestialWorld;

Player player;
Terrain terrain;

Window * window;
InputHandler * input;

long long t = 0;

void draw()
{
	celestialWorld.setTime(nowMicro() * 600);

	window->Clear();
	celestialWorld.drawPerspective(player.getCamera());

	glClear(GL_DEPTH_BUFFER_BIT);
	glm::vec4 dir(0.0f);
	glm::vec4 color(0.0f);
	if (celestialWorld.getNumLights() > 0) {
		dir = celestialWorld.getLightDir(0);
		color = celestialWorld.getLightColor(0);
	}
	terrain.setLight(dir, color);
	terrain.Draw(player.getCamera());

	window->Swap();
}

void mainLoop()
{
	Event event;
	int quit = 0;
	int deltaIndex = 0;
	while (!input->quit) {
		input->Reset();
		while (PollEvent(&event)) {
			input->HandleEvents(&event, window);
			if (input->windowEvent) {
				// CelestialWorlds needs to know about window resizing
				celestialWorld.setAspect(float(input->windowWidth) / float(input->windowHeight));
			}
		}

		std::chrono::duration<float> delta = getDelta();
		player.updateCamera(delta);
		player.maintainBounds();
		player.handleJumps(delta);
//		printf("camera dir: [%5.2f][%5.2f][%5.2f]\n", player.getCamera().dir.x, player.getCamera().dir.y, player.getCamera().dir.z);
		draw();
		
/*		float deltas[10];
		deltas[deltaIndex] = delta.count();
		deltaIndex = (deltaIndex + 1) % 10;
		float fps = 0.0f;
		for (unsigned int i = 0; i < 10; i++)
			fps += deltas[i];
		fps /= 10.0f;
		printf("fps: %f\n", 1.0f / fps);
*/	}
}

int main(int argc, char ** argv)
{
	const char * systemFile;
	if (argc < 2)
		systemFile = DEFAULT_SYSTEM_FILE;
	else
		systemFile = argv[1];
	
	const char * bodyFile;
	if (argc < 3)
		bodyFile = "Earth";
	else
		bodyFile = argv[2];

	window = new Window();
	input = new InputHandler();

	celestialWorld.Init();
	if (!(celestialWorld.loadSystemFile(systemFile))) {
		delete window;
		delete input;
		return 1;
	}
	celestialWorld.setAspect(float(input->windowWidth) / float(input->windowHeight));
	celestialWorld.putCameraOnBody(bodyFile);
	
	terrain.Init();
	
	player.Init(&terrain, input);

//	celestialWorld.setTime(100000000000000);
	mainLoop();
	return 0;
}
