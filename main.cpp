#include <cstdio>
#include "world.h"
#include "window.h"
#include "groundplane.h"

Window * window;
InputHandler * input;
World * world;
GroundPlane groundplane;

void play()
{
	long long time = 0; // time since Jan 1, 1970 in microseconds
	Camera * camera = world->GetCamera();
	glm::mat4 rotationMatrix;
	int celestialID = 3;
	while (!input->HandleEvents(window)) {
		if (!input->paused)
			time += 16000;
		world->Update(time);

		if (input->lastKey == SDLK_F6)
			celestialID = (celestialID == 3) ? -1 : 3; /* Rosa or -1 */

		window->Clear();
		glDisable(GL_BLEND);
		world->DrawPerspective(celestialID);
		glEnable(GL_BLEND);
		if (celestialID == 3)
			groundplane.Draw(camera);
		window->Swap();
	}
}

int main(int argc, char ** argv)
{
	const char * systemFile;

	if (argc < 2)
		systemFile = "flips.txt";
	else
		systemFile = argv[1];

	window = new Window();
	input = new InputHandler();
	world = new World(window, input);
	world->Init(systemFile);
	groundplane.Init();

	play();

	return 0;
}
