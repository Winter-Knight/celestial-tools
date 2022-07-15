#include <cstdio>
#include "world.h"
#include "window.h"
#include "groundplane.h"
#include "parser.h"

Window * window;
InputHandler * input;
World * world;
GroundPlane groundplane;

const char * systemFile;

void play()
{
	long long time = 31557600000000; // one year after epoch
	Camera * camera = world->GetCamera();
	glm::mat4 rotationMatrix;
	int celestialID = -1;
//	float timeSpeed = 10000.0f;
	float timeSpeed = 1.0f;
	const unsigned char * keyboardState = SDL_GetKeyboardState(NULL);
	while (!input->HandleEvents(window)) {
		if (!input->paused)
			time += 16000 * timeSpeed;
		world->Update(time);

		if (input->lastKey == SDLK_F6)
			celestialID = (celestialID == 3) ? -1 : 3; /* Rosa or -1 */
		if (input->lastKey == SDLK_SEMICOLON)
			printf("[%5.2f][%5.2f][%5.2f]\n", camera->pos.x, camera->pos.y, camera->pos.z);
		if (keyboardState[SDL_SCANCODE_KP_PLUS] || keyboardState[SDL_SCANCODE_EQUALS]) {
			timeSpeed += 100.0f;
			printf("time speed: %f\n", timeSpeed);
		}
		if (keyboardState[SDL_SCANCODE_KP_MINUS] || keyboardState[SDL_SCANCODE_MINUS]) {
			timeSpeed -= 100.0f;
			printf("time speed: %f\n", timeSpeed);
		}
		if (input->lastKey == SDLK_HOME) {
			*camera = *getCameraFromFile(systemFile);
			camera->CalculateView();
			camera->aspect = float(input->windowWidth) / float(input->windowHeight);
			camera->CalculatePerspective();
		}
			


		window->Clear();
		world->DrawPerspective(celestialID);
		if (celestialID == 3)
			groundplane.Draw(camera);
		window->Swap();
	}
}

int main(int argc, char ** argv)
{
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
