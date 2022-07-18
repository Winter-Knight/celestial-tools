#include <cstdio>

#include <glm/glm.hpp>
#include <celestialworlds.hpp>

#include "timefuncs.h"

#include "sdl/window.h"
#include "sdl/input.h"

#include "player.h"
#include "terrain.h"

celestial::World celestialWorld;

Player player;
Terrain terrain;

Window * window;
InputHandler * input;

void draw()
{
	celestialWorld.setTime(nowMicro() * 600);

	window->Clear();
	celestialWorld.drawPerspective(player.getCamera());
	glClear(GL_DEPTH_BUFFER_BIT);
	terrain.Draw(player.getCamera());
	window->Swap();
}

void mainLoop()
{
	SDL_Event event;
	int quit = 0;
	int deltaIndex = 0;
	while (!input->quit) {
		input->Reset();
		while (SDL_PollEvent(&event)) {
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
		systemFile = "../sol.txt";
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
	celestialWorld.loadSystemFile(systemFile);
	celestialWorld.setAspect(float(input->windowWidth) / float(input->windowHeight));
	celestialWorld.putCameraOnBody(bodyFile);
	
	terrain.Init();
	glm::vec4 pos(0.0f);
	glm::vec4 color(0.0f);
	if (celestialWorld.getNumLights() > 0) {
		pos = celestialWorld.getLightPos(0);
		color = celestialWorld.getLightColor(0);
	}
	terrain.setLight(pos, color);
	
	player.Init(&terrain, input);

	mainLoop();
	return 0;
}
